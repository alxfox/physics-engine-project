#include "CustomScenarios.h"
#include "engine/utils.h"
#include "gui/graphics/Material.h"

#include <cmath>
#include <iostream>

gp::Custom1::Custom1()
{
    gp::graphics::Material& green = getMaterial("green");
    green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
    gp::graphics::Material& blue = getMaterial("blue");
    blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);
    //Entity e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(4.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
    Entity e = addBox(1.0f, engine::Vector3f(4.0f, -1.5f, 0.0f), engine::Vector3f::Ones(),engine::Vector3f(0.f,5.f,0.f));
    setMaterial(e, blue);
    e= addSphere(1.0f, engine::Vector3f(3.4f, 0.0f, 0.0f), 0.7f);
    setMaterial(e, blue);
}

gp::Custom2::Custom2()
{
    gp::graphics::Material& orange = getMaterial("orange");
	orange.diffuseColor = engine::Vector3f(0.7f, 0.6f, 0.6f);

    gp::graphics::Material& myBlue = getMaterial("myBlue");
	myBlue.diffuseColor = engine::Vector3f(0.2f, 0.6f, 1.0f);

    gp::graphics::Material& white = getMaterial("xd");
    white.diffuseColor = engine::Vector3f(0.0f, 0.0f, 1.0f);
	white.shininess = 10.0f;
	white.specularColor = engine::Vector3f(0.2f, 1.0f, 1.0f);

    Entity e = addBox(gp::engine::Object::UNMOVABLE_MASS, //mass
                engine::Vector3f(2, 0, 0), //position
                engine::Vector3f::Ones()*1, //size 
                engine::Vector3f(0, 0, 0), //velocity
        		engine::Quaternion(M_PI / 6, engine::Vector3f(0, 1, 0)) //rotation
                );
	setMaterial(e, myBlue);

    std::string colorName;
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
    }
	//setName(e, "orange box")
}

gp::Custom3::Custom3()
{
  
}
