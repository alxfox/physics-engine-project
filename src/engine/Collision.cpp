#include <cassert>
#include <cmath>
#include <limits>
#include <iostream>
#include <ostream>

#include "Collision.h"
#include "config.h"
#include "math/AABox.h"
#include "math/BoxProjection.h"
#include "math/Line.h"
#include "math/utils.h"
#include "utils.h"

bool gp::engine::Collision::detect()
{
	bool ret = false;


	switch (m_type) {
	case SPHERE_SPHERE:
		ret = detectSphereSphere();
		break;
	case SPHERE_BOX:
		ret = detectSphereBox();
		break;
	case BOX_BOX:
		ret = detectBoxBox();
		break;
	case CONSTRAINT:
		// Get collision data from the constraint
		assert(m_constraint);
		ret = m_constraint->collision(m_collisionNormal, m_collisionPoint1, m_collisionPoint2,
			m_interpenetrationDepth);
		break;
	}

	assert(!ret || !std::isnan(m_interpenetrationDepth));

	return ret;
}

bool gp::engine::Collision::detectSphereSphere()
{
	Sphere* sphere1 = dynamic_cast<Sphere*>(m_object1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(m_object2);
	float_t collDistance = sphere1->radius() + sphere2->radius(); //if distance is smaller than this value then there is a collision
	Vector3f collNormal = sphere2->position() - sphere1->position(); //normal vector from sphere1 to sphere2
	float_t collNormalLength = collNormal.norm();
	if(collNormalLength < collDistance){
		m_collisionNormal = collNormal.normalized();
		assert(abs(m_collisionNormal.norm() - 1) < EPSILON);
		m_collisionPoint1 = sphere1->position()+collNormal.normalized()*sphere1->radius();
		m_collisionPoint2 = sphere2->position()-collNormal.normalized()*sphere2->radius();
		m_interpenetrationDepth=collDistance-collNormalLength;
		return true;
	}
	return false;
}

bool gp::engine::Collision::detectSphereBox()
{
	Box* myBox= dynamic_cast<Box*>(m_object2);
	Sphere* mySphere = dynamic_cast<Sphere*>(m_object1);

	AABox aabox (*myBox);
	Vector3f sphereLocation = myBox->invModelMatrix() * mySphere->position(); // transform coordinates of the sphere in world space to the box's model space
	Vector3f boxSurfacePoint = aabox.closestPointOnSurface(sphereLocation);
	Vector3f collNormalOld = (boxSurfacePoint-sphereLocation); // normal goes from the sphere center to the surface point of the box
	Vector3f collNormal = myBox->modelMatrix().linear()*collNormalOld;//convert the collision normal to world space (length is not affected because of linear)
	float_t collNormalLength = collNormal.norm();
	collNormal = (collNormal).normalized();
	if(collNormalLength < mySphere->radius()){
		//convert everything back to world space
		m_collisionPoint1 = myBox->modelMatrix()*sphereLocation + mySphere->radius()*collNormal;
		m_collisionPoint2 = myBox->modelMatrix()*boxSurfacePoint;
		m_collisionNormal = collNormal; // was already converted to world space
		m_interpenetrationDepth = mySphere->radius()-collNormalLength;

		//Just assert that the distance from the center of the sphere to the plane, is bigger or equal than the radius
		Vector3f collisionPointToCenter = mySphere->position() - m_collisionPoint1;
		float_t distSpherePlane = abs(m_collisionNormal.dot(collisionPointToCenter));
		assert(distSpherePlane >= mySphere->radius()-EPSILON);

		//Just assert that the 8 corners of a box lie on the same side
		int pointsAbove = 0;
		int pointsBelow = 0;
		for (int i = -1; i < 2; i+=2){
			for (int j = -1; j < 2; j+=2){
				for (int k = -1; k < 2; k+=2){
					Vector3f boxCorner = Vector3f(i*myBox->halfSize().x(), j*myBox->halfSize().y(), k*myBox->halfSize().z());
					boxCorner = myBox->modelMatrix()*boxCorner;
					Vector3f collisionPointToCorner = boxCorner - m_collisionPoint2;
					float_t dist = collisionPointToCorner.dot(m_collisionNormal);
					if(dist >= -EPSILON) {
						pointsAbove+=1;
					}
					if(dist < EPSILON) {
						pointsBelow+=1;
					}
				}
			}
		}
		assert(pointsBelow==8 or pointsAbove==8);
		return true;
	}
	return false;
}



void boxAxes(const gp::engine::Box &box, gp::engine::Vector3f axes[3])
	{
		axes[0] = box.modelMatrix().matrix().block<3, 1>(0, 0);
		axes[1] = box.modelMatrix().matrix().block<3, 1>(0, 1);
		axes[2] = box.modelMatrix().matrix().block<3, 1>(0, 2);
	}

gp::engine::Vector3f center2center(const gp::engine::Box &box1, const gp::engine::Box &box2)
	{
		return box2.position() - box1.position();
	}




bool gp::engine::Collision::detectBoxBox()
{
  // TODO

// TODO
	Box box1 = *dynamic_cast<Box*>(m_object1);
	Box box2 = *dynamic_cast<Box*>(m_object2);

	Vector3f boxAxis1[3]; 
	Vector3f boxAxis2[3];

	boxAxes(box1, boxAxis1);
	boxAxes(box2, boxAxis2);

	//std::cout << boxAxis1[0] << std::endl;
	//std::cout << boxAxis2[0] << std::endl;
	Vector3f c2c = center2center(box1, box2);
	BoxProjection boxProj(boxAxis1, box1.halfSize(), boxAxis2, box2.halfSize(), c2c);

	Vector3f projectionNormal;
	float_t overlap;
	float_t minOverlap = std::numeric_limits<float_t>::max();
	

	bool isEdge = false;
	//========================================Checking principal axes 3+3========================================
	for (int i = 0; i < 3; i++){
		projectionNormal = boxAxis1[i];
		overlap = boxProj.overlapOnAxis(projectionNormal);
		//if(overlap < 0) {
		//	std::cout << "\n=\n" << projectionNormal << "\n===\n" <<std::endl;
		//}
		//std::cout << overlap << std::endl;
		if (overlap <= 0){
			return false;
		}
		if(overlap < minOverlap){
			minOverlap = overlap;
			m_collisionNormal = projectionNormal;
		}
	}
	for (int i = 0; i < 3; i++){
		projectionNormal = boxAxis2[i];
		overlap = boxProj.overlapOnAxis(projectionNormal);
		if (overlap <= 0){
			return false;
		}
		if(overlap < minOverlap){
			minOverlap = overlap;
			m_collisionNormal = projectionNormal;
		}
	}
	//========================================Checking 3x3 Combinations========================================
	int box1EdgeIndex, box2EdgeIndex;
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			projectionNormal = boxAxis1[i].cross(boxAxis2[j]).normalized();
			overlap = boxProj.overlapOnAxis(projectionNormal);
			if (overlap <= 0){
				return false;
			}
			if(overlap < minOverlap){
				minOverlap = overlap;
				m_collisionNormal = projectionNormal;
				//Saving data for edge-edge collision
				isEdge = true;
				box1EdgeIndex = i;
				box2EdgeIndex = j;	
			}
		}
	}

	//std::cout << "overlap: " << minOverlap<< std::endl;
	//std::cout << "Overlapped!!!!" << std::endl;
	m_interpenetrationDepth = minOverlap;
	
	//------------------->Assure collision normal points from o1 to o2---------------------
	if((box2.position() - (box1.position() + m_collisionNormal*m_interpenetrationDepth)).norm()
		> (box2.position() - (box1.position())).norm()){
		m_collisionNormal = -m_collisionNormal;
	}  


	//========================================Collision Types========================================
	if(!isEdge){
		for (int i = -1; i < 2; i+=2){
			for (int j = -1; j < 2; j+=2){
				for (int k = -1; k < 2; k+=2){
					Vector3f bC1 = box1.position() + i*boxAxis1[0]*box1.halfSize().x() + j*boxAxis1[1]*box1.halfSize().y() + k*boxAxis1[2]*box1.halfSize().z();
					Vector3f bC2 = box2.position() + i*boxAxis2[0]*box2.halfSize().x() + j*boxAxis2[1]*box2.halfSize().y() + k*boxAxis2[2]*box2.halfSize().z();
					//Point inside iff all the coordinates are in absolute value smaller than the corresponding half size
					bC1 = box2.invModelMatrix()*bC1;
					if((abs(bC1(0)) < box2.halfSize()(0) && abs(bC1(1)) < box2.halfSize()(1) && abs(bC1(2)) < box2.halfSize()(2))){	
						m_collisionPoint1 = box2.modelMatrix()*bC1;
						m_collisionPoint2 = m_collisionPoint1 - m_collisionNormal*m_interpenetrationDepth; 
					}
					bC2 = box1.invModelMatrix()*bC2;
					if((abs(bC2(0)) < box1.halfSize()(0) && abs(bC2(1)) < box1.halfSize()(1) && abs(bC2(2)) < box1.halfSize()(2))){							
						m_collisionPoint2 = box1.modelMatrix()*bC2;
						m_collisionPoint1 = m_collisionPoint2 + m_collisionNormal*m_interpenetrationDepth; 
					}
				}
			}
		}

	return true;

	}
	else{
		float_t minDistance = std::numeric_limits<float_t>::max();
		Vector3f edge1 = boxAxis1[box1EdgeIndex];
		Vector3f edge2 = boxAxis2[box2EdgeIndex];
		for (int i = -1; i < 2; i+=2){
			for (int j = -1; j < 2; j+=2){
				for (int k = -1; k < 2; k+=2){
					Vector3f bC1 = box1.position() + i*boxAxis1[0]*box1.halfSize().x() + j*boxAxis1[1]*box1.halfSize().y() + k*boxAxis1[2]*box1.halfSize().z();
					Line box1Line(bC1, edge1);
					for (int i2 = -1; i2 < 2; i2+=2){
						for (int j2 = -1; j2 < 2; j2+=2){
							for (int k2 = -1; k2 < 2; k2+=2){
								Vector3f bC2 = box2.position() + i2*boxAxis2[0]*box2.halfSize().x() + j2*boxAxis2[1]*box2.halfSize().y() + k2*boxAxis2[2]*box2.halfSize().z();
								Line box2Line(bC2, edge2);
								Vector3f point1;
								Vector3f point2;
								if(box1Line.closestPoints(box2Line, point1, point2)){							
									if ((point1 - point2).norm() < minDistance){
										minDistance = (point1 - point2).norm();
										m_collisionPoint1 = point1;
										m_collisionPoint1 = point2;
									}
								}
							}
						}
					}

				}
			}
		}
		return true;
	}


	return false;



  
//   Box* myBox1= dynamic_cast<Box*>(m_object1);
//   Box* myBox2= dynamic_cast<Box*>(m_object2);
//   Vector3f xAxis(1,0,0);
//   Vector3f yAxis(0,1,0);
//   Vector3f zAxis(0,0,1);
//   Vector3f box1Axis[]={(myBox1->modelMatrix()*xAxis).normalized(),(myBox1->modelMatrix()*yAxis).normalized(),(myBox1->modelMatrix()*zAxis).normalized()};
//   Vector3f box2Axis[]={(myBox2->modelMatrix()*xAxis).normalized(),(myBox2->modelMatrix()*yAxis).normalized(),(myBox2->modelMatrix()*zAxis).normalized()};
//   BoxProjection bp = BoxProjection(box1Axis,myBox1->halfSize(),box2Axis,myBox2->halfSize(),myBox2->position()-myBox1->position());
//   float_t minOverlap = std::numeric_limits<float_t>::max();
//   Vector3f minOverlapAxis;
//   Vector3f edge1;
//   Vector3f edge2;
//   bool isEdge = false; //whether the min overlap is an edge-edge collision
//   Vector3f currentAxis;
//   float_t currentOverlap;
//   // 6 = 3+3 | 3 axis per box to check

//   int edgeFromBox;
//   for (int i = 0; i < 3; i++)
//   {
//     currentAxis=box1Axis[i];
//     currentOverlap = bp.overlapOnAxis(currentAxis);
//     if(currentOverlap<minOverlap){
//       minOverlap = currentOverlap;
//       minOverlapAxis = currentAxis;
//       isEdge = false;
//       edgeFromBox = i;
//       edge1 = currentAxis;
//       if(minOverlap<EPSILON) return false;
//     }

//     currentAxis=box2Axis[i];
//     currentOverlap = bp.overlapOnAxis(currentAxis);
//     if(currentOverlap<minOverlap){
//       minOverlap = currentOverlap;
//       minOverlapAxis = currentAxis;
//       isEdge = false;
//       edgeFromBox = 3 + i;
//             edge1 = currentAxis;
//       if(minOverlap<EPSILON) return false;
//     }
//   }
//   // 9 = 3*3 | check cross products of all combinations of axis between the two boxes
//   size_t id1, id2;

//   for (int i = 0; i < 3; i++){
//     for (int k = 0; k < 3; k++){
//       currentAxis=(box1Axis[i].cross(box2Axis[k])).normalized();
//       currentOverlap = bp.overlapOnAxis(currentAxis);
//       if(currentOverlap<minOverlap){
//         minOverlap = currentOverlap;
//         minOverlapAxis = currentAxis;
//         isEdge = true;
//         edge1=box1Axis[i];
//         edge2=box2Axis[k];
//         id1 = i;
//         id2 = k;
//         if(minOverlap<EPSILON) return false;
//       }
//     }
//   }

//   // check collision normal direction and correct it if necessary
//   Vector3f centerDiff = myBox2->position() - myBox1->position();
//   float_t checkResult = centerDiff.dot(minOverlapAxis);
//   if (checkResult < 0) {
//       minOverlapAxis = -minOverlapAxis;
//   }

//   //minOverlap > EPSILON
//   if(!isEdge){//vertex-plane collision
//         Vector3f point1, point2, closestPoint1, closestPoint2;        
//         Vector3f minPoint1, minPoint2;
//         float_t minDist = std::numeric_limits<float_t>::max(), possibleDist;


// for (int i = -1; i < 2; i += 2) {
//             for (int j = -1; j < 2; j += 2) {
//                 for (int k = -1; k < 2; k += 2 ) {
//                     for (int l = -1; l < 2; l += 2) {
//                         Vector3f noX = i*yAxis*myBox1->halfSize()(1) + j*zAxis*myBox1->halfSize()(2);
//                         Vector3f noY = i*xAxis*myBox1->halfSize()(0) + j*zAxis*myBox1->halfSize()(2);
//                         Vector3f noZ = i*xAxis*myBox1->halfSize()(0) + j*yAxis*myBox1->halfSize()(1);
//                         if (id1 == 0) {
//                             point1 = myBox1->modelMatrix()*noX;
//                         }
//                         if (id1 == 1) {
//                             point1 = myBox1->modelMatrix()*noY;
//                         }
//                         if (id1 == 2) {
//                             point1 = myBox1->modelMatrix()*noZ;
//                         }
//                         noX = k*yAxis*myBox2->halfSize()(1) + l*zAxis*myBox2->halfSize()(2);
//                         noY = k*xAxis*myBox2->halfSize()(0) + l*zAxis*myBox2->halfSize()(2);
//                         noZ = k*xAxis*myBox2->halfSize()(0) + l*yAxis*myBox2->halfSize()(1);
//                         if (id2 == 0) {
//                             point2 = myBox2->modelMatrix()*noX;
//                         }
//                         if (id2 == 1) {
//                             point2 = myBox2->modelMatrix()*noY;
//                         }
//                         if (id2 == 2) {
//                             point2 = myBox2->modelMatrix()*noZ;
//                         }
//                         Line l1(point1, edge1), l2(point2, edge2);
//                         l1.closestPoints(l2, closestPoint1, closestPoint2);
                        
//                         possibleDist = (closestPoint1 - closestPoint2).norm(); 
                        
//                         if (possibleDist < minDist) {
//                             minPoint1 = closestPoint1;
//                             minPoint2 = closestPoint2;
//                         }
//                     }
//                 }
//             }
//         }
        
//         m_collisionNormal = minOverlapAxis;
//         m_collisionPoint1 = minPoint1;
//         m_collisionPoint2 = minPoint2;
//         m_interpenetrationDepth = minOverlap;
//         return true;
//   }
//   else {
//       m_collisionNormal = edge1;
//       m_interpenetrationDepth = minOverlap;

//         Vector3f possiblePoint, minPoint;
//         float_t minDist = std::numeric_limits<float_t>::max(), possibleDist;

// if (edgeFromBox < 3) {
//           for (int i = -1; i < 2; i += 2) {
//               for (int j = -1; j < 2; j += 2) {
//                   for (int k = -1; k < 2; k +=2 ) {
//                         possiblePoint = myBox2->modelMatrix() *
//                                 (xAxis*myBox2->halfSize()(0)
//                                 + yAxis*myBox2->halfSize()(1)
//                                 + zAxis*myBox2->halfSize()(2));
//                         possibleDist = (myBox1->position() - possiblePoint).norm();
//                         if (possibleDist < minDist) {
//                             minPoint = possiblePoint;
//                             minDist = possibleDist;
//                         }
//                   }
//               }
//           }
//           m_collisionPoint2 = minPoint;
//           m_collisionPoint1 = minPoint - minOverlapAxis*minOverlap;
//       }
//       else {
//             for (int i = -1; i < 2; i += 2) {
//                 for (int j = -1; j < 2; j += 2) {
//                     for (int k = -1; k < 2; k +=2 ) {
//                         possiblePoint = myBox1->modelMatrix() *
//                                         (xAxis*myBox1->halfSize()(0)
//                                          + yAxis*myBox1->halfSize()(1)
//                                          + zAxis*myBox1->halfSize()(2));
//                         possibleDist = (myBox2->position() - possiblePoint).norm();
//                         if (possibleDist < minDist) {
//                             minPoint = possiblePoint;
//                             minDist = possibleDist;
//                         }
//                     }
//                 }
//             }
//             m_collisionPoint1 = minPoint;
//             m_collisionPoint2 = minPoint + minOverlapAxis*minOverlap;
//         }

//   }
//   return false;
}



gp::engine::Collision::CollisionType gp::engine::Collision::getType(gp::engine::Object *o1, gp::engine::Object *o2)
{
	if (dynamic_cast<Sphere*>(o1)) {
		if (dynamic_cast<Sphere*>(o2))
			return SPHERE_SPHERE;
		return SPHERE_BOX;
	} else {
		if (dynamic_cast<Sphere*>(o2))
			return SPHERE_BOX;
		return BOX_BOX;
	}
}

gp::engine::Object *gp::engine::Collision::getFirstObject(gp::engine::Object *o1, gp::engine::Object *o2)
{
	if (dynamic_cast<Sphere*>(o1))
		return o1;
	return o2;
}

gp::engine::Object *gp::engine::Collision::getSecondObject(gp::engine::Object *o1, gp::engine::Object *o2)
{
	if (dynamic_cast<Sphere*>(o1))
		return o2;
	return o1;
}