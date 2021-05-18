#include "CustomScenarios.h"
#include "gui/graphics/Material.h"

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
  
}

gp::Custom3::Custom3()
{
  
}
