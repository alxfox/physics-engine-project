#include "CustomScenarios.h"
#include "gui/graphics/Material.h"

gp::Custom1::Custom1()
{
    
	gp::graphics::Material& welt = getMaterial("welt");
	gp::graphics::Material& beton = getMaterial("beton");
	beton.setTexture(gp::graphics::Material::Diffuse, "beton_3");
    
	Entity fixedSphere2 = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, 2, 0), 0.4);
	setMaterial(fixedSphere2, beton);

	Entity e = addBox(1, engine::Vector3f(0.0, 1.0, 0.0), engine::Vector3f::Ones()* 0.5,engine::Vector3f(0, 0, 0),
	engine::Quaternion(M_PI, engine::Vector3f(0, 0, 1)));
	engine::Object* o = m_objectManager.engine.find(e);
	o->setAngularVelocity(engine::Vector3f(0, 0, M_PI));
	setMaterial(e, welt);

	addRope(fixedSphere2, e, 1.5f, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));

	Entity box2 = addBox(1, engine::Vector3f(0.0, 0.0, 0.0), engine::Vector3f::Ones()* 0.5,engine::Vector3f(0, 0, 0),
	engine::Quaternion(M_PI, engine::Vector3f(0, 0, 1)));
	engine::Object* u = m_objectManager.engine.find(box2);
	u->setAngularVelocity(engine::Vector3f(0, 0, -M_PI));
	setMaterial(box2, welt);

	addRope(e, box2, 1.5f, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));



}

gp::Custom2::Custom2()
{
  gp::graphics::Material& green = getMaterial("green");
    green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
    gp::graphics::Material& blue = getMaterial("blue");
    blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);;
    gp::graphics::Material& beton = getMaterial("beton");
    beton.setTexture(gp::graphics::Material::Diffuse, "beton_3");

    //Box rechter rand springt Hoch
    Entity e = addBox(1.0f, engine::Vector3f(4.0f, -1.5f, 0.0f), engine::Vector3f::Ones(),engine::Vector3f(-1.f,10.f,0.f));
    setMaterial(e, blue);
    //Sphere Rechts über Box 
    e = addSphere(1.0f, engine::Vector3f(3.3f, 0.0f, 0.0f), 0.7f);
    setMaterial(e, blue);
   
    //Linke seite oben
    e = addSphere(1.0f, engine::Vector3f(0.f, 2.0f, -0.9f), 0.7f);
    setMaterial(e, blue);
    //Linke seite unten
    e = addSphere(3.0f, engine::Vector3f(-0.1, -1, -1.0), 1.f, engine::Vector3f(0.f,7.f,0.f));
    setMaterial(e, green);

    //kugel schwer vorne
    e = addSphere(7.0f, engine::Vector3f(-3, -1.5, 2), 0.5f, engine::Vector3f(5.f,0.f,0.f));
    setMaterial(e, beton);
    // //kugel leicht vorne
    e = addSphere(3.0f, engine::Vector3f(2, -1, 2.0), 1.f, engine::Vector3f(-5.f,0.f,0.f));
    setMaterial(e, green);

    //box from air coming middle
    e = addBox(1.0f, engine::Vector3f(0, 5, 0), engine::Vector3f::Ones());
    setMaterial(e, blue);
}

gp::Custom3::Custom3()
{
  
gp::graphics::Material& blue = getMaterial("blue");
	blue.diffuseColor = engine::Vector3f(0.0f, 0.0f, 1.0f);
	blue.shininess = 10.0f;
	blue.specularColor = engine::Vector3f(0.2f, 1.0f, 1.0f);
	gp::graphics::Material& green = getMaterial("green");
	green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.5f);
	green.setTexture(gp::graphics::Material::Diffuse, "beton_3");
	green.setTexture(gp::graphics::Material::NormalMap, "beton_3Normal");
	gp::graphics::Material& orange = getMaterial("orange");
	orange.diffuseColor = engine::Vector3f(1.0f, 0.64f, 0.0f);

	gp::graphics::Material& boden = getMaterial("boden");
	boden.setTexture(gp::graphics::Material::Diffuse, "boden_12");
	gp::graphics::Material& white = getMaterial("white");
	


	

  //sphere fest links
	Entity fixedSphere = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(-0.4, 3, 0), 0.4);
	setMaterial(fixedSphere, white);
  //sphere on rope left
	Entity e = addSphere(1, engine::Vector3f(-2.15, 1.5, 0), 0.4);
	setMaterial(e, green);

	addRope(fixedSphere, e, 2.305, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));

  //sphere fest rechts
	Entity fixedSphere2 = addSphere(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.4, 3, 0), 0.4);
	setMaterial(fixedSphere2, white);
  //sphere on rope right
	e = addSphere(1, engine::Vector3f(2.15, 1.5, 0), 0.4);
	setMaterial(e, green);

	addRope(fixedSphere2, e, 2.305, engine::Vector3f(0.0f, 0.0f, 0.0f), engine::Vector3f(0.0f, 0.0f, 0.0f));
  



	//kugel vorne links
	Entity r = addSphere(7.0f, engine::Vector3f(-0.5, 1, 2), 0.5f, engine::Vector3f(-5.f,0.f,0.f));
   	setMaterial(r, green);

	//kugeln vorne rechts
  	e = addSphere(7.0f, engine::Vector3f(0.5, 1, 2), 0.5f, engine::Vector3f(5.f,0.f,0.f));
   	setMaterial(e, green);

	 //rope
	 addRope(r, e, 3, engine::Vector3f(0.0f, 0, 0.0f),engine::Vector3f(0.0f, 0, 0.0f));
	 
}
