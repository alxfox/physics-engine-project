#include "Scenarios.h"
#include "gui/graphics/Material.h"
#include <sstream>
gp::Scenario1::Scenario1()
{
	// gp::graphics::Material& blue = getMaterial("blue");
	// blue.diffuseColor = engine::Vector3f(0.0f, 0.0f, 1.0f);
	// blue.shininess = 10.0f;
	// blue.specularColor = engine::Vector3f(0.2f, 1.0f, 1.0f);
	// gp::graphics::Material& green = getMaterial("green");
	// green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
	// green.setTexture(gp::graphics::Material::Diffuse, "beton_3");
	// green.setTexture(gp::graphics::Material::NormalMap, "beton_3Normal");
	// gp::graphics::Material& orange = getMaterial("orange");
	// orange.diffuseColor = engine::Vector3f(1.0f, 0.64f, 0.0f);

	// gp::graphics::Material& boden = getMaterial("boden");
	// boden.setTexture(gp::graphics::Material::Diffuse, "boden_12");
	// gp::graphics::Material& white = getMaterial("white");
	// gp::graphics::Material& welt = getMaterial("welt");


	// Entity e = addBox(1, engine::Vector3f::Zero());
	// setMaterial(e, green);
	// setName(e, "green box");

	// e = addSphere(1, engine::Vector3f(0.0, 2.0, 0.0), 0.3);
	// setMaterial(e, white);
	// setName(e, "white ball");

	// e = addBox(1, engine::Vector3f(-1.7, 0, 1.7), engine::Vector3f::Ones()*0.5, engine::Vector3f(2, 0, 0),
	// 	engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
	// engine::Object* o = m_objectManager.engine.find(e);
	// o->setAngularVelocity(engine::Vector3f(M_PI, 0, 0));
	// setMaterial(e, welt);
	// setName(e, "red box");

	// e = addSphere(1, engine::Vector3f(2.7, 0, 0), 0.3, engine::Vector3f(0, 0, 0));
	// setMaterial(e, welt);
	// setName(e, "blue ball");

	// e = addSphere(1, engine::Vector3f(2.7, 3, 0), 0.5, engine::Vector3f(0, 0, 0));
	// setMaterial(e, welt);
	// setName(e, "red ball");

	// e = addBox(1, engine::Vector3f(0.5, 5, 0), engine::Vector3f::Ones()*0.5, engine::Vector3f(0, 0, 0),
	// 	engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
	// setMaterial(e, orange);
	// setName(e, "orange box");

	// Entity fixedSphere = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, 3, -4), 0.4);
	// setMaterial(fixedSphere, white);

	// e = addSphere(1, engine::Vector3f(0.2, 1.5, -4), 0.4);
	// setMaterial(e, green);

	// addRope(fixedSphere, e, 2, engine::Vector3f(0.0f, -0.4f, 0.0f), engine::Vector3f(0.0f, 0.4f, 0.0f));

	// Entity fixedSphere2 = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(1, 3, -4), 0.4);
	// setMaterial(fixedSphere2, white);

	// e = addBox(1, engine::Vector3f(3.2, 1.5, -4));
	// setMaterial(e, green);

	// addRope(fixedSphere2, e, 1.5f, engine::Vector3f(0.0f, -0.4f, 0.0f), engine::Vector3f::Ones() * 0.5f);
}

gp::BoxBoxTest::BoxBoxTest()
{
  gp::graphics::Material& green = getMaterial("green");
  green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
  gp::graphics::Material& blue = getMaterial("blue");
  blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);
  
  Entity e;
	e = addBox(1, engine::Vector3f(-3.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
  setMaterial(e, blue);
	e = addBox(1, engine::Vector3f(-3.5f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(0, 0, 0), 
    engine::Quaternion(M_PI * 0.25, engine::Vector3f(1, 0, 0)) * engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
  setMaterial(e, green);
  
	e = addBox(1, engine::Vector3f(-1.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
  setMaterial(e, blue);
	e = addBox(1, engine::Vector3f(-1.0f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(0, 0, 0), engine::Quaternion(M_PI * 0.125, engine::Vector3f(0, 0, 1)));
  setMaterial(e, green);
  
	e = addBox(1, engine::Vector3f(1.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
  setMaterial(e, blue);
	e = addBox(1, engine::Vector3f(1.0f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(0, 0, 0), engine::Quaternion(0.0f, engine::Vector3f(0, 0, 1)));
  setMaterial(e, green);
  
	e = addBox(1, engine::Vector3f(3.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
  setMaterial(e, blue);
	e = addBox(1, engine::Vector3f(3.0f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(0, 0, 0),
    engine::Quaternion(M_PI * 0.25, engine::Vector3f(1, 0, 0)) * engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
  setMaterial(e, green);
}

gp::ManySpheres::ManySpheres()
{
	gp::graphics::Material& green = getMaterial("green");
	green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
	gp::graphics::Material& blue = getMaterial("blue");
	blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);
	blue.shininess = 200.0f;

	float const scale = 0.5f;
	float const spacing = 2.f;
	unsigned const numSpheres = 5;
	for (unsigned x = 0; x < numSpheres; ++x) {
		for (unsigned y = 0; y < numSpheres; ++y) {
			for (unsigned z = 0; z < numSpheres; ++z) {
				Entity e = addSphere(1, engine::Vector3f(spacing*x - numSpheres/2 * spacing, spacing*y, spacing*z - numSpheres/2 * spacing), scale);
				if ((x+y+z) % 2) {
					setMaterial(e, green);
				} else {
					setMaterial(e, blue);
				}
			}
		}
	}

	Entity e = addSphere(16.0f, engine::Vector3f(0.1f, 20.0f, 0.0f), 4.0f);
	setMaterial(e, getMaterial("welt"));
}

gp::ManyCubes::ManyCubes()
{
	gp::graphics::Material& cube1 = getMaterial("cube1");
	cube1.setTexture(gp::graphics::Material::Diffuse, "holz_15");
	cube1.setTexture(gp::graphics::Material::NormalMap, "holz_15Normal");
	gp::graphics::Material& cube2 = getMaterial("cube2");
	cube2.setTexture(gp::graphics::Material::Diffuse, "rost_8");
	cube2.setTexture(gp::graphics::Material::NormalMap, "rost_8Normal");

	float const scale = 1.0f;
	float const spacing = 2.f;
	unsigned const numBoxes = 5;
	for (unsigned x = 0; x < numBoxes; ++x) {
		for (unsigned y = 0; y < numBoxes; ++y) {
			for (unsigned z = 0; z < numBoxes; ++z) {
				Entity e = addBox(1, engine::Vector3f(spacing*x - numBoxes/2 * spacing, spacing*y, spacing*z - numBoxes/2 * spacing), engine::Vector3f::Ones() * scale);
				if ((x+y+z) % 2) {
					setMaterial(e, cube1);
				} else {
					setMaterial(e, cube2);
				}
			}
		}
	}

	Entity e = addBox(10.0f, engine::Vector3f(20.0f, 15.0f, 70.0f), engine::Vector3f::Ones(), engine::Vector3f(-10.0f, 0.0f, -40.0f));
	setMaterial(e, getMaterial("welt"));
}


gp::HangingBridge::HangingBridge()
{
  gp::graphics::Material& cube = getMaterial("cube");
  cube.setTexture(gp::graphics::Material::Diffuse, "wand_18");
  cube.setTexture(gp::graphics::Material::NormalMap, "wand_18Normal");
  cube.shininess = 125.0f;
  gp::graphics::Material& welt = getMaterial("welt");
  welt.shininess = 300.0f;
  welt.specularColor = engine::Vector3f::Ones() * 0.8f;
  
  float arenaSize = 10.0f;
  unsigned const numX = 10;
  unsigned const numZ = 5;
  float const spacingZ = 1.0f;
  float const scale = 0.2f;
  float const ropeLengthX = 0.9f;
  float const ropeLengthZ = spacingZ;
  Entity entities[numX][numZ];
  for (unsigned x = 0; x < numX; ++x) {
    float xpos = -arenaSize/2.0 + static_cast<float>(x)/(numX-1) * arenaSize;
    for (unsigned z = 0; z < numZ; ++z) {
      float mass = (x == 0 || x == numX-1) ? gp::engine::Object::UNMOVABLE_MASS : 1.0;
      entities[x][z] = addBox(mass, engine::Vector3f(xpos, 1.0f, spacingZ*z - numZ/2 * spacingZ), engine::Vector3f::Ones() * scale);
      std::stringstream tag;
	  tag << "x: " << x << " z: "<<z;
	  setName(entities[x][z],tag.str());
	  setMaterial(entities[x][z], cube);
      if (x > 0) {
        addRope(entities[x-1][z], entities[x][z], ropeLengthX, engine::Vector3f(scale/2.0, 0.0f, 0.0f), engine::Vector3f(-scale/2.0, 0.0f, 0.0f));
      }
      if (z > 0) {
        addRope(entities[x][z-1], entities[x][z], ropeLengthZ, engine::Vector3f(0.0f, 0.0f, scale/2.0), engine::Vector3f(0.0f, 0.0f, -scale/2.0));
      }
    }
    if (x%3 == 1) {  
      Entity e = addSphere(16.0f, engine::Vector3f(xpos, 10.0f, 0.0f), 1.0f);
      setMaterial(e, getMaterial("welt"));
    }
  }
}

gp::Pendulum::Pendulum()
{
  gp::graphics::Material& beton = getMaterial("beton");
  beton.setTexture(gp::graphics::Material::Diffuse, "beton_3");
  beton.setTexture(gp::graphics::Material::NormalMap, "beton_3Normal");
  gp::graphics::Material& welt = getMaterial("welt");
  welt.setTexture(gp::graphics::Material::Diffuse, "welt");
  
  unsigned const numX = 7;
  float const radius = 0.5f;
  float const ropeLength = 6.0f;
  Entity e1, e2;
  for (unsigned x = 0; x < numX; ++x) {
    float xpos1 = 2.0 * radius * x - numX/2 * 2.0 * radius;
    float xpos2 = xpos1;
    float ypos1 = -2.0f;
    float ypos2 = ypos1+ropeLength;
    engine::Quaternion rot;
    if (x == numX-1) {
      xpos1 += sin(M_PI * 0.25) * ropeLength;
      ypos1 += ropeLength * (1.0f - cos(M_PI * 0.25));
      rot = engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1));
    }
    e1 = addSphere(10.0f, engine::Vector3f(xpos1, ypos1, 0.0f), radius, engine::Vector3f::Zero(), rot);
    setMaterial(e1, beton);
    e2 = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(xpos2, ypos2, 0.0f), radius/2.0f);
    setMaterial(e2, welt);
    addRope(e1, e2, ropeLength-1.5f*radius, engine::Vector3f(0.0f, radius, 0.0f), engine::Vector3f(0.0f, -radius/2.0f, 0.0f));
  }
}


gp::StandardArena::StandardArena()
{
	gp::graphics::Material& orange = getMaterial("orange");
	orange.diffuseColor = engine::Vector3f(1.0f, 0.64f, 0.1f);
	gp::graphics::Material& white = getMaterial("white");

	gp::graphics::Material& welt = getMaterial("welt");
	welt.setTexture(gp::graphics::Material::Diffuse, "welt");

	float arenaSize = 30.0f;
	float arenaHeight = 8.0f;
	float arenaOffset = -2.0f;
	//float arenaThickness = 0.1f;
	float arenaThickness = 0.1f;
	Entity e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(-arenaSize/2.0, arenaOffset + arenaHeight/2.0, 0), engine::Vector3f(arenaThickness, arenaHeight, arenaSize));
	setMaterial(e, orange);
	setName(e, "left");

	e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(arenaSize/2.0, arenaOffset + arenaHeight/2.0, 0), engine::Vector3f(arenaThickness, arenaHeight, arenaSize));
	setMaterial(e, orange);
	setName(e, "right");

	e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0, arenaOffset + arenaHeight/2.0, -arenaSize/2.0), engine::Vector3f(arenaSize, arenaHeight, arenaThickness));
	setMaterial(e, orange);
	setName(e, "back");

	e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0, arenaOffset + arenaHeight/2.0, arenaSize/2.0), engine::Vector3f(arenaSize, arenaHeight, arenaThickness));
	setMaterial(e, orange);
	setName(e, "front");

	e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, -2, 0), engine::Vector3f(2.0f*arenaSize, arenaThickness, 2.0f*arenaSize));
	setMaterial(e, white);
	setName(e, "bottom");

	e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, arenaHeight+4, 0), engine::Vector3f(2.0f*arenaSize, arenaThickness, 2.0f*arenaSize));
	setMaterial(e, white);
	setName(e, "top");

	m_spotLight.setupLight(glm::vec3(0.0f, 12.0f, 7.0f), glm::vec3(0.0f, -1.0f, -0.5f), 30.0f, 0.1f);
	m_camera.setWorldPosition(glm::vec3(0.0f, -1.0f, 10.0f));
}
