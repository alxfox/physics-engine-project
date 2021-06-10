#include <cxxtest/TestSuite.h>

#include "engine/Collision.h"
class CollisonTest : public CxxTest::TestSuite
{
public:
// TODO: Expand testing of Collision
	void testDetectSphereSphere(void)
	{
        gp::engine::Sphere sphere(1., gp::engine::Vector3f(3.2f, 0.0f, 0.0f));
		gp::engine::Box box(1.,gp::engine::Vector3f(2.0f, -0.1f, 0.0f));
		gp::engine::Object* o1 = &sphere;
		gp::engine::Object* o2 = &box;
		gp::engine::Collision coll(o1, o2);
		bool ret = coll.detect();
		TS_ASSERT_EQUALS(ret,true);
		// collPoint is on the sphere
		TS_ASSERT_DELTA((coll.collisionPoint1()-sphere.position()).norm(),sphere.radius(),gp::engine::EPSILON);
	}
    void testdetectSphereBox(void)
	{
		gp::engine::Sphere sphere(1., gp::engine::Vector3f(3.4f, 0.0f, 0.0f));
		gp::engine::Box box(1.,gp::engine::Vector3f(4.0f, -0.5f, 0.0f));
		gp::engine::Object* o1 = &sphere;
		gp::engine::Object* o2 = &box;
		gp::engine::Collision coll(o1, o2);
		bool ret = coll.detect();
		TS_ASSERT_EQUALS(ret,true);
		// collPoint is on the sphere
		//std::cout<<"This is a Test "  << coll.collisionPoint1();
		TS_ASSERT_DELTA((coll.collisionPoint1()-sphere.position()).norm(),sphere.radius(),gp::engine::EPSILON);
		// interpentration depth is distance between collision points
		TS_ASSERT_DELTA((coll.collisionPoint2()-coll.collisionPoint1()).norm(),coll.interpenetrationDepth(),gp::engine::EPSILON);
		//collNormal is normalized
		TS_ASSERT_DELTA(coll.collisionNormal().norm(),1.,gp::engine::EPSILON);
		//TS_ASSERT(false);
	}

    void testdetectBoxBox(void)
	{/*
		gp::engine::Box box1(1., gp::engine::Vector3f(3.4f, 0.0f, 0.0f));
		gp::engine::Box box2(1.,gp::engine::Vector3f(4.0f, -0.5f, 0.0f));
		gp::engine::Object* o1 = &box1;
		gp::engine::Object* o2 = &box2;
		gp::engine::Collision coll(o1, o2);
		
*/
		//Vertex-Plane Collision
		gp::engine::Box box1(1, gp::engine::Vector3f(3.0f,-1.48002934f, 0.0f), gp::engine::Vector3f::Ones());
		gp::engine::Box box2(1, gp::engine::Vector3f(3.0f, -.130854562f, 0.0f), gp::engine::Vector3f::Ones(), gp::engine::Vector3f(0, 0, 0),
    	gp::engine::Quaternion(M_PI * 0.25, gp::engine::Vector3f(1, 0, 0)) * gp::engine::Quaternion(M_PI * 0.25, gp::engine::Vector3f(0, 0, 1)));
		gp::engine::Object* o1 = &box1;
		gp::engine::Object* o2 = &box2;
		gp::engine::Collision coll1(o1, o2);
		helperVPBoxBox(box1,box2,coll1);

		//Edge-Edge Collision
		gp::engine::Box box3(1, gp::engine::Vector3f(-2.79874992f, -1.47997916f, 0.0f), gp::engine::Vector3f::Ones());	//bottom box
		gp::engine::Box box4(1, gp::engine::Vector3f(-3.70125008f, -0.411302239f, 0.0f), gp::engine::Vector3f::Ones(), gp::engine::Vector3f(0, 0, 0), //top box
    	gp::engine::Quaternion(M_PI * 0.25, gp::engine::Vector3f(1, 0, 0)) * gp::engine::Quaternion(M_PI * 0.25, gp::engine::Vector3f(0, 0, 1)));
		gp::engine::Object* o3 = &box3;
		gp::engine::Object* o4 = &box4;
		gp::engine::Collision coll2(o3, o4);
		helperEEBoxBox(box3,box4,coll2);

		//"Plane-Plane"(treated as Vertex-Plane but the collision points will be wrong because no vertex is in the other box)
		gp::engine::Box box5(1, gp::engine::Vector3f(-3.5f, -1.48002934f, -3.f), gp::engine::Vector3f::Ones());
	  	gp::engine::Box box6(1, gp::engine::Vector3f(-3.5f, -0.73335278f, -3.0f), gp::engine::Vector3f(2.0f, 0.5f, 0.3f));
		gp::engine::Object* o5 = &box5;
		gp::engine::Object* o6 = &box6;
		gp::engine::Collision coll3(o5, o6);
		bool ret = coll3.detect();
		TS_ASSERT_EQUALS(ret,true);
		//collisionNormal is normalized
		TS_ASSERT_DELTA(coll3.collisionNormal().norm(),1.,gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll3.collisionNormal().y(),-1.f,gp::engine::EPSILON);
	}
	private:
	static void helperVPBoxBox(const gp::engine::Box &box1,const gp::engine::Box &box2, gp::engine::Collision coll)
	{
		bool ret = coll.detect();
		TS_ASSERT_EQUALS(ret,true);
		//collisionNormal is normalized
		TS_ASSERT_DELTA(coll.collisionNormal().norm(),1.,gp::engine::EPSILON);

		gp::engine::Vector3f normal = coll.collisionPoint1()-coll.collisionPoint2();
		float_t interpen = normal.norm();
		//interpentration depth is distance between collision points
		TS_ASSERT_DELTA(coll.interpenetrationDepth(),interpen,gp::engine::EPSILON);

		//normal points from point 2 to point 1
		normal.normalize();
		TS_ASSERT_DELTA(coll.collisionNormal().x(),normal.x(),gp::engine::EPSILON);
		//std::cout <<"Hello World: "<<coll.collisionPoint2()<<" htest "<<coll.collisionPoint1()<< coll.collisionNormal().y() << " "<<normal.y()<<" end of world"<<std::endl;
		TS_ASSERT_DELTA(coll.collisionNormal().y(),normal.y(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().z(),normal.z(),gp::engine::EPSILON);

		//one collision point is inside one box
		bool oneCollInsideBox = false;
		gp::engine::Vector3f coll1inBox1 = box1.invModelMatrix()*coll.collisionPoint1();
		if((abs(coll1inBox1(0)) < box1.halfSize()(0) && abs(coll1inBox1(1)) < box1.halfSize()(1) && abs(coll1inBox1(2)) < box1.halfSize()(2))){
			oneCollInsideBox=true;
		}
		gp::engine::Vector3f coll2inBox1 = box1.invModelMatrix()*coll.collisionPoint2();
		if((abs(coll2inBox1(0)) < box1.halfSize()(0) && abs(coll2inBox1(1)) < box1.halfSize()(1) && abs(coll2inBox1(2)) < box1.halfSize()(2))){
			oneCollInsideBox=true;
		}
		gp::engine::Vector3f coll1inBox2 = box2.invModelMatrix()*coll.collisionPoint1();
		if((abs(coll1inBox2(0)) < box2.halfSize()(0) && abs(coll1inBox2(1)) < box2.halfSize()(1) && abs(coll1inBox2(2)) < box2.halfSize()(2))){	
			oneCollInsideBox=true;
		}
		gp::engine::Vector3f coll2inBox2 = box2.invModelMatrix()*coll.collisionPoint2();
		if((abs(coll2inBox2(0)) < box2.halfSize()(0) && abs(coll2inBox2(1)) < box2.halfSize()(1) && abs(coll2inBox2(2)) < box2.halfSize()(2))){	
			oneCollInsideBox=true;
		}
					
		TS_ASSERT_EQUALS(oneCollInsideBox,true);

		//one collision point is on a box plane
		gp::engine::Vector3f xyz[] = {{1,0,0},{0,1,0},{0,0,1}};
		gp::engine::Vector3f collinside[] = {coll1inBox1,coll2inBox1,coll1inBox2,coll2inBox2};
		gp::engine::Vector3f boxHalfSizes[] = {box1.halfSize(),box2.halfSize()};
		bool oneCollOnAPlane = false;
		for(int i = 0; i<2;i++){
			for(int k = 0; k<2;k++){
			oneCollOnAPlane = oneCollOnAPlane || (abs(abs(xyz[0].dot(collinside[k+i*2]))-boxHalfSizes[i][0])<gp::engine::EPSILON 
						&& abs(xyz[1].dot(collinside[k+i*2]))-boxHalfSizes[i][1]<gp::engine::EPSILON
						&& abs(xyz[2].dot(collinside[k+i*2]))-boxHalfSizes[i][2]<gp::engine::EPSILON);
			oneCollOnAPlane = oneCollOnAPlane || (abs(abs(xyz[1].dot(collinside[k+i*2]))-boxHalfSizes[i][1])<gp::engine::EPSILON 
						&& abs(xyz[0].dot(collinside[k+i*2]))-boxHalfSizes[i][0]<gp::engine::EPSILON
						&& abs(xyz[2].dot(collinside[k+i*2]))-boxHalfSizes[i][2]<gp::engine::EPSILON);
			oneCollOnAPlane = oneCollOnAPlane || (abs(abs(xyz[2].dot(collinside[k+i*2]))-boxHalfSizes[i][2])<gp::engine::EPSILON 
						&& abs(xyz[1].dot(collinside[k+i*2]))-boxHalfSizes[i][1]<gp::engine::EPSILON
						&& abs(xyz[0].dot(collinside[k+i*2]))-boxHalfSizes[i][0]<gp::engine::EPSILON);
			}
		}
		TS_ASSERT_EQUALS(oneCollOnAPlane,true);
	}
	static void helperEEBoxBox(const gp::engine::Box &box1,const gp::engine::Box &box2, gp::engine::Collision coll)
	{
		bool ret = coll.detect();
		TS_ASSERT_EQUALS(ret,true);
		//collisionNormal is normalized
		TS_ASSERT_DELTA(coll.collisionNormal().norm(),1.,gp::engine::EPSILON);
		//std::cout <<"Hello World: "<<std::endl<<"P2"<<std::endl<<coll.collisionPoint2()<<std::endl<<"P1"<<std::endl<<coll.collisionPoint1()<<" end of world"<<std::endl;
		gp::engine::Vector3f normal = coll.collisionPoint1()-coll.collisionPoint2();
		float_t interpen = normal.norm();
		//interpentration depth is distance between collision points
		TS_ASSERT_DELTA(coll.interpenetrationDepth(),interpen,gp::engine::EPSILON);
/*
		//normal points from point 2 to point 1
		normal.normalize();
		TS_ASSERT_DELTA(coll.collisionNormal().x(),normal.x(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().y(),normal.y(),gp::engine::EPSILON);
		TS_ASSERT_DELTA(coll.collisionNormal().z(),normal.z(),gp::engine::EPSILON);
*/
		//both collision points are on an edge of a box
		gp::engine::Vector3f coll1inBox1 = box1.invModelMatrix()*coll.collisionPoint1();
		gp::engine::Vector3f coll2inBox1 = box1.invModelMatrix()*coll.collisionPoint2();
		gp::engine::Vector3f coll1inBox2 = box2.invModelMatrix()*coll.collisionPoint1();
		gp::engine::Vector3f coll2inBox2 = box2.invModelMatrix()*coll.collisionPoint2();
		gp::engine::Vector3f xyz[] = {{1,0,0},{0,1,0},{0,0,1}};
		gp::engine::Vector3f collinside[] = {coll1inBox1,coll2inBox1,coll1inBox2,coll2inBox2};
		gp::engine::Vector3f boxHalfSizes[] = {box1.halfSize(),box2.halfSize()};
		bool collOnEdge[] = {false,false};//both collision points have to be on an edge
		for(int i = 0; i<2;i++){//which box
			for(int k = 0; k<2;k++){//which point
			collOnEdge[k] = collOnEdge[k] || (abs(abs(xyz[0].dot(collinside[k+i*2]))-boxHalfSizes[i][0])<gp::engine::EPSILON 
						&& abs(abs(xyz[1].dot(collinside[k+i*2]))-boxHalfSizes[i][1])<gp::engine::EPSILON
						&& abs(xyz[2].dot(collinside[k+i*2]))-boxHalfSizes[i][2]<gp::engine::EPSILON);
			collOnEdge[k] = collOnEdge[k] || (abs(abs(xyz[0].dot(collinside[k+i*2]))-boxHalfSizes[i][0])<gp::engine::EPSILON 
						&& abs(xyz[1].dot(collinside[k+i*2]))-boxHalfSizes[i][1]<gp::engine::EPSILON
						&& abs(abs(xyz[2].dot(collinside[k+i*2]))-boxHalfSizes[i][2])<gp::engine::EPSILON);
			collOnEdge[k] = collOnEdge[k] || (abs(xyz[0].dot(collinside[k+i*2]))-boxHalfSizes[i][0]<gp::engine::EPSILON 
						&& abs(abs(xyz[1].dot(collinside[k+i*2]))-boxHalfSizes[i][1])<gp::engine::EPSILON
						&& abs(abs(xyz[2].dot(collinside[k+i*2]))-boxHalfSizes[i][2])<gp::engine::EPSILON);
			}
		}
		TS_ASSERT_EQUALS(collOnEdge[0],true);
		TS_ASSERT_EQUALS(collOnEdge[1],true);
	}
};
