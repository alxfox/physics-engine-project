#include "CustomScenarios.h"
#include "gui/graphics/Material.h"

gp::Custom1::Custom1()
{
    gp::graphics::Material& green = getMaterial("green");
    green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
    gp::graphics::Material& blue = getMaterial("blue");
    blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);
    Entity e = addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(4.0f, -1.5f, 0.0f), engine::Vector3f::Ones());
    setMaterial(e, blue);
    e= addSphere(1.0f, engine::Vector3f(3.4f, 0.0f, 0.0f), 0.7f);
    setMaterial(e, blue);
}

gp::Custom2::Custom2()
{
    gp::graphics::Material& orange = getMaterial("orange");
	orange.diffuseColor = engine::Vector3f(0.7f, 0.6f, 0.6f);

    gp::graphics::Material& myBlue = getMaterial("blue");
	myBlue.diffuseColor = engine::Vector3f(0.0f, 0.2f, 1.0f);

    Entity e = addBox(gp::engine::Object::UNMOVABLE_MASS,
                engine::Vector3f(2, 0, 0), 
                engine::Vector3f::Ones()*1, 
                engine::Vector3f(0, 0, 0),
        		engine::Quaternion(M_PI / 6, engine::Vector3f(0, 1, 0))
                );
	setMaterial(e, myBlue);
	//setName(e, "orange box")
}

gp::Custom3::Custom3()
{
  
}
