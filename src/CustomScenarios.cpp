#include "CustomScenarios.h"
#include "engine/utils.h"
#include "gui/graphics/Material.h"

#include <cmath>
#include <iostream>

gp::Custom1::Custom1()
{	
	//shows that the Quaternion is working right

	gp::graphics::Material& green = getMaterial("green");
  green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
  gp::graphics::Material& blue = getMaterial("blue");
  blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);
	blue.shininess = 10.0f;
	blue.specularColor = engine::Vector3f(0.2f, 1.0f, 1.0f);
	
	green.setTexture(gp::graphics::Material::Diffuse, "beton_3");
	green.setTexture(gp::graphics::Material::NormalMap, "beton_3Normal");
	gp::graphics::Material& orange = getMaterial("orange");
	orange.diffuseColor = engine::Vector3f(1.0f, 0.64f, 0.0f);
	gp::graphics::Material& boden = getMaterial("boden");
	boden.setTexture(gp::graphics::Material::Diffuse, "boden_12");
	gp::graphics::Material& white = getMaterial("white");
  

  

/*PENDULAR RIGHT BACK
*/

  //sphere fix left
	Entity fixedSphere = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(1.6, 3, -2.0), 0.4);
	setMaterial(fixedSphere, orange);
  //sphere on rope left
	Entity e = addSphere(3, engine::Vector3f(-0.15, 1.5, -2.0), 0.4);
	setMaterial(e, green);

	addRope(fixedSphere, e, 2.305, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));

  //sphere fix right
	Entity fixedSphere2 = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(2.4, 3, -2.0), 0.4);
	setMaterial(fixedSphere2, orange);
  //sphere on rope right
	e = addSphere(3, engine::Vector3f(4.15, 1.5, -2.0), 0.4);
	setMaterial(e, green);
	
	//rope
	addRope(fixedSphere2, e, 2.305, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));


/*PENDULAR LEFT
*/

//sphere fix left
	Entity fixedSphere3 = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(-2.4, 3, -2.0), 0.4);
	setMaterial(fixedSphere3, orange);
  //sphere on rope left
	e = addSphere(3, engine::Vector3f(-4.15, 1.5, -2.0), 0.4);
	setMaterial(e, green);

	addRope(fixedSphere3, e, 2.305, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));




/*SPHERE IN FRONT 
*/
 e = addSphere(7.0f, engine::Vector3f(-3, -1.5, 2), 0.5f, engine::Vector3f(7.f,0.f,0.f));
    setMaterial(e, green);

/*BOXES LEFT SIDE
*/

  //boxes leftleft
	e = addBox(1, engine::Vector3f(-3.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
  setMaterial(e, blue);
	e = addBox(1, engine::Vector3f(-3.5f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(4.0, 1, 0), 
    engine::Quaternion(M_PI * 0.25, engine::Vector3f(1, 0, 0)) * engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
  setMaterial(e, white);
  //boxes left
	e = addBox(1, engine::Vector3f(-1.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
  setMaterial(e, blue);
	e = addBox(1, engine::Vector3f(-1.0f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(-3.0, 3, 0), engine::Quaternion(M_PI * 0.125, engine::Vector3f(0, 0, 1)));
  setMaterial(e, white);

/*Spinning cube
*/
gp::graphics::Material& welt = getMaterial("welt");
e = addBox(1, engine::Vector3f(-1.7, 0, 1.7), engine::Vector3f::Ones()*0.5, engine::Vector3f(2, 0, 0),
		engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
	engine::Object* o = m_objectManager.engine.find(e);
	o->setAngularVelocity(engine::Vector3f(M_PI, 0, 0));
	setMaterial(e, welt);
	


//   //boxes right
// 	e = addBox(1, engine::Vector3f(1.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
//   setMaterial(e, blue);
// 	e = addBox(1, engine::Vector3f(1.0f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(3.0, 3, 0), engine::Quaternion(0.0f, engine::Vector3f(0, 0, 1)));
//   setMaterial(e, green);
//   //boxes right right
// 	e = addBox(1, engine::Vector3f(3.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
//   setMaterial(e, blue);
// 	e = addBox(1, engine::Vector3f(3.0f, 0.0f, 0.0f), engine::Vector3f::Ones(), engine::Vector3f(-4.0, 1, 0),
//     engine::Quaternion(M_PI * 0.25, engine::Vector3f(1, 0, 0)) * engine::Quaternion(M_PI * 0.25, engine::Vector3f(0, 0, 1)));
//   setMaterial(e, green);



}

gp::Custom2::Custom2()
{

	//shows every possible collision with impulse

		//texture
  gp::graphics::Material& green = getMaterial("green");
    green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
    gp::graphics::Material& blue = getMaterial("blue");
    blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);;
    gp::graphics::Material& beton = getMaterial("beton");
    beton.setTexture(gp::graphics::Material::Diffuse, "beton_3");

    //box right side jumping up 
    Entity e = addBox(1.0f, engine::Vector3f(4.0f, -1.5f, 0.0f), engine::Vector3f::Ones(),engine::Vector3f(-1.f,10.f,0.f));
    setMaterial(e, blue);
    //Sphere right above box
    e = addSphere(1.0f, engine::Vector3f(3.3f, 0.0f, 0.0f), 0.7f);
    setMaterial(e, blue);
   
    //sphere right lower side
    e = addSphere(1.0f, engine::Vector3f(0.f, 2.0f, -0.9f), 0.7f);
    setMaterial(e, blue);
    //sphere left lower side 
    e = addSphere(3.0f, engine::Vector3f(-0.1, -1, -1.0), 1.f, engine::Vector3f(0.f,7.f,0.f));
    setMaterial(e, green);
	//sphere heavy front
    e = addSphere(7.0f, engine::Vector3f(-3, -1.5, 2), 0.5f, engine::Vector3f(5.f,0.f,0.f));
    setMaterial(e, beton);
    //sphere light front
    e = addSphere(3.0f, engine::Vector3f(2, -1, 2.0), 1.f, engine::Vector3f(-5.f,0.f,0.f));
    setMaterial(e, green);

    //box from air coming middle
    e = addBox(1.0f, engine::Vector3f(0, 5, 0), engine::Vector3f::Ones());
    setMaterial(e, blue);

   // std::string colorName;
   // for(int i = 0; i < 40; ++i){

   //     e = addBox(gp::engine::Object::UNMOVABLE_MASS,
   //             engine::Vector3f(0, i*0.2f - 1, 0), 
   //             engine::Vector3f(1.0f, 0.1f, 0.2f),
   //             engine::Vector3f(0, 0, 0),
   //     		engine::Quaternion((2*M_PI / 40) * i, engine::Vector3f(0, 1, 0))
   //             );

   //     colorName = "c" + std::to_string(i);
   //     getMaterial(colorName).diffuseColor = engine::Vector3f(0.2f, 0.5f, i/40.0f);
	  //  getMaterial(colorName).shininess = 10.0f;
	  //  getMaterial(colorName).specularColor = engine::Vector3f(0.2f, 0.5f, i/40.0f);
   //     //gp::engine::Vector3f a;
   //     //a << 0.1f, 0.1f, 0.1f;
	  //  setMaterial(e, getMaterial(colorName));
   // }


    //int j = 0;
    //for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/40){

    //    e = addBox(gp::engine::Object::UNMOVABLE_MASS,
    //            engine::Vector3f(std::cos(degree)*1.1f, 3, -std::sin(degree)*1.1f), 
    //            gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
    //            //engine::Vector3f(2.0f, 0.1f, 0.2f),
    //            engine::Vector3f(0, 0, 0),
    //    		engine::Quaternion((2*M_PI / 40) * j, engine::Vector3f(0, 1, 0))
    //            );

    //    colorName = "c" + std::to_string(j);
    //    getMaterial(colorName).diffuseColor = engine::Vector3f(0.2f, 0.5f, j/40.0f);
	    // getMaterial(colorName).shininess = 10.0f;
	    // getMaterial(colorName).specularColor = engine::Vector3f(0.2f, 0.5f, j/40.0f);
    //    //gp::engine::Vector3f a;
    //    //a << 0.1f, 0.1f, 0.1f;
	    // setMaterial(e, getMaterial(colorName));
    //    j+=1;
    //}
	//setName(e, "orange box")
    //sphere heavy front
    
}

gp::Custom3::Custom3()
{

  
	//shows interaction with ropes and the impulse towards two spheres and in different directions with rope

  //textures

  gp::graphics::Material& green = getMaterial("green");
  green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
  gp::graphics::Material& blue = getMaterial("blue");
  blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);
  
  Entity e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.5f, 0.25f, 0.0f), engine::Vector3f(.4f, 0.4f, 0.4f), engine::Vector3f::Zero(),
	engine::Quaternion(), engine::Object::TRIGGER_PLAYER);

  std::string colorName;
  int j = 0;
  float_t n = 20.0;
  for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){

      e = addSphere(1,//gp::engine::Object::UNMOVABLE_MASS,
              engine::Vector3f(std::cos(degree)*8.1f, 8, 1-std::sin(degree)*8.1f), 
              0.3f,//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
              //engine::Vector3f(2.0f, 0.1f, 0.2f),
              engine::Vector3f(0, 0, 0),
      		    engine::Quaternion((2*M_PI / n) * j, engine::Vector3f(0, 1, 0))
              );

      colorName = "c" + std::to_string(j);
      getMaterial(colorName).diffuseColor = engine::Vector3f(0.2f, 0.5f, j/n);
	 /// getMaterial(colorName).shininess = 10.0f;
	 /// getMaterial(colorName).specularColor = engine::Vector3f(0.2f, 0.5f, j/40.0f);
      //gp::engine::Vector3f a;
      //a << 0.1f, 0.1f, 0.1f;
	    setMaterial(e, getMaterial(colorName));
      j+=1;
  }

}
