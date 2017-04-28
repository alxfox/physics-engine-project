#include <iostream>

#include "Scenario.h"
#include "engine/objects/Box.h"
#include "engine/objects/Sphere.h"
#include "engine/constraints/Rope.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

gp::Entity gp::Scenario::addBox(float_t mass, const engine::Vector3f &position, const engine::Vector3f &size,
		const engine::Vector3f &velocity, const engine::Quaternion &rotation)
{
	// Create the visualization object
	Entity entity = addVisObject(gp::graphics::Cube, position, 0.5*size, rotation);

	// Set engine manifestation
	m_objectManager.engine.add(entity, gp::engine::Box(mass, position, size, velocity, rotation));

	return entity;
}

gp::Entity gp::Scenario::addSphere(float_t mass, const gp::engine::Vector3f &position, float_t radius,
		const gp::engine::Vector3f &velocity, const engine::Quaternion &rotation)
{
	// Create the visualization object
	Entity entity = addVisObject(gp::graphics::Sphere, position, engine::Vector3f(radius, radius, radius), rotation);

	// Set engine manifestation
	m_objectManager.engine.add(entity, gp::engine::Sphere(mass, position, radius, velocity, rotation));

	return entity;
}

gp::Entity gp::Scenario::addVisObject(gp::graphics::Shape shape, const engine::Vector3f &position, const engine::Vector3f &size,
	const engine::Quaternion &rotation)
{
	// Create the entity
	Entity entity = m_entityManager.create();

	// Set visualization manifestation
	graphics::RenderComponentManager::Instance inst = m_objectManager.render.add(entity);
	m_objectManager.render.setShape(inst, shape);
	glm::mat4 rotationMatrix = glm::make_mat4(rotation.rotationMatrix().data());
	m_objectManager.render.setModelMatrix(inst, glm::translate(glm::vec3(position.x(), position.y(), position.z()))
			* rotationMatrix);
	m_objectManager.render.setScale(inst, glm::vec3(size.x(), size.y(), size.z()));
  m_objectManager.render.setMaterial(inst, &getMaterial("default"));

	return entity;
}

gp::Entity gp::Scenario::addRope(Entity object1, Entity object2, engine::float_t length,
		const engine::Vector3f &objPoint1, const engine::Vector3f &objPoint2)
{
	// Create the entity
	Entity entity = m_entityManager.create();

	// Get physics objects
	engine::Object* o1 = m_objectManager.engine.find(object1);
	engine::Object* o2 = m_objectManager.engine.find(object2);
	if (o1 == m_objectManager.engine.end() || o2 == m_objectManager.engine.end()) {
		std::cerr << "Scenario: Could not find objects for new constraint" << std::endl;
		m_entityManager.destroy(entity);
		return entity; // Return a dead entity
	}

	// Create the physics constraint
	m_constraintManager.engine.add(entity, engine::Rope(*o1, *o2, length, objPoint1, objPoint2));
  m_constraintManager.render.add(entity);

	return entity;
}


gp::graphics::Material& gp::Scenario::getMaterial(std::string const& name)
{
  auto it = m_materials.find(name);
  if (it == m_materials.end()) {
    m_materials[name] = new gp::graphics::Material;
    it = m_materials.find(name);
  }
	return *it->second;
}

void gp::Scenario::setName(Entity entity, const std::string &name)
{
	engine::Object* o = m_objectManager.engine.find(entity);
	if (o == m_objectManager.engine.end()) {
		std::cerr << "Scenario: Could not set name to unknown object" << std::endl;
		return;
	}

	o->setName(name);
}

void gp::Scenario::setMaterial(Entity entity, gp::graphics::Material& material)
{
	graphics::RenderComponentManager::Instance inst = m_objectManager.render.find(entity);
	if (inst == m_objectManager.render.end()) {
		std::cerr << "Scenario: Could not set material for unknown object" << std::endl;
		return;
	}

	m_objectManager.render.setMaterial(inst, &material);
}


void gp::Scenario::synchronize()
{
	// Iterate over all render objects
	for (graphics::RenderComponentManager::EntityIterator it = m_objectManager.render.entitiesBegin();
		it != m_objectManager.render.entitiesEnd(); ++it) {
		engine::Object* o = m_objectManager.engine.find(*it);
		if (o == m_objectManager.engine.end())
			continue;

		graphics::RenderComponentManager::Instance inst = m_objectManager.render.find(*it);

		// Update the position and rotation
		glm::mat4 modelMatrix = glm::make_mat4(o->modelMatrix().data());
		m_objectManager.render.setModelMatrix(inst, modelMatrix);
	}
  
  for (graphics::ConstraintManager::EntityIterator it = m_constraintManager.render.entitiesBegin();
		it != m_constraintManager.render.entitiesEnd(); ++it) {
		engine::HardConstraint* o = m_constraintManager.engine.find(*it);
		if (o == m_constraintManager.engine.end())
			continue;

		graphics::ConstraintManager::Instance inst = m_constraintManager.render.find(*it);

		// Update the position and rotation
    m_constraintManager.render.setLine(inst, o->computeCollisionPoint1(), o->computeCollisionPoint2());
	}
}
