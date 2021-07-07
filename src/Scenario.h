#ifndef GP_SCENARIO_H
#define GP_SCENARIO_H

#include <string>

#include "common/EntityManager.h"
#include "engine/ObjectManager.h"
#include "engine/ConstraintManager.h"
#include "engine/utils.h"
#include "engine/math/Quaternion.h"
#include "gui/graphics/Camera.h"
#include "gui/graphics/Material.h"
#include "gui/graphics/ConstraintManager.h"
#include "gui/graphics/RenderComponentManager.h"
#include "gui/graphics/SpotLight.h"

namespace gp
{

/**
 * @brief Stores all objects
 */
struct ObjectManager
{
	/** The engine component manager */
	gp::engine::ObjectManager engine;

	/** The graphics component manager */
	gp::graphics::RenderComponentManager render;
};

/**
 * @brief Stores all constraints
 */
struct ConstraintManager
{
	/** The engine component manager */
	gp::engine::ConstraintManager engine;

 	/** The graphics component manager */
 	gp::graphics::ConstraintManager render;
};

/**
 * @brief Main class for all scenarios
 *
 * Holds the information about all objects in the current scene.
 */
class Scenario
{
protected:
	/** The manager for all entities */
	gp::EntityManager m_entityManager;

	/** The objects of this scenario */
	ObjectManager m_objectManager;

	/** The (hard) constraints of this scenario */
	ConstraintManager m_constraintManager;

	/** Spot light */
	gp::graphics::SpotLight m_spotLight;

	/** Camera */
	gp::graphics::Camera m_camera;

	/** Stores materials */
	std::unordered_map<std::string, gp::graphics::Material*> m_materials;
public:
	virtual ~Scenario()
	{
		for (auto it = m_materials.begin(); it != m_materials.end(); ++it) {
			delete it->second;
		}
	}

	/**
	 * @return The entity of the new box
	 */
	Entity addBox(float_t mass = 1, const engine::Vector3f &position = engine::Vector3f::Zero(),
		const engine::Vector3f &size = engine::Vector3f::Ones(), const engine::Vector3f &velocity = engine::Vector3f::Zero(),
		const engine::Quaternion &rotation = engine::Quaternion(), bool trig = false);

	/**
	 * @return The entity of the new sphere
	 */
	Entity addSphere(float_t mass = 1, const engine::Vector3f &position = engine::Vector3f::Zero(),
		float_t radius = 1, const engine::Vector3f &velocity = engine::Vector3f::Zero(),
		const engine::Quaternion &rotation = engine::Quaternion(), bool trig = false);

	/**
	 * Add a visualization object
	 *
	 * @return The entity of the new object
	 */
	Entity addVisObject(gp::graphics::Shape shape,
		const engine::Vector3f &position = engine::Vector3f::Zero(),
		const engine::Vector3f &size = engine::Vector3f::Ones(),
		const engine::Quaternion &rotation = engine::Quaternion());

	/**
	 * Add a rope between to objects
	 *
	 * @return The entity of the new constraint
	 */
	Entity addRope(Entity object1, Entity object2, engine::float_t length = 3,
		const engine::Vector3f &objPoint1 = engine::Vector3f::Zero(),
		const engine::Vector3f &objPoint2 = engine::Vector3f::Zero());

	/**
	 * Get a material by name. Creates a new material when it does not exist already.
	 *
	 * @return Reference to material object.
	 */
	gp::graphics::Material& getMaterial(std::string const& name);

	/**
	 * Set the name of an engine object (for debugging)
	 */
	void setName(Entity entity, const std::string &name);

	/**
	 * Set the material for a visualization object
	 */
	void setMaterial(Entity entity, gp::graphics::Material& material);

	/**
	 * @return The engine object manager
	 */
	gp::engine::ObjectManager& engineObjectManager()
	{
		return m_objectManager.engine;
	}

	/**
	 * @return The render object manager
	 */
	gp::graphics::RenderComponentManager& renderObjectManager()
	{
		return m_objectManager.render;
	}

	/**
	 * @return The engine constraint manager
	 */
	gp::engine::ConstraintManager& engineConstraintManager()
	{
		return m_constraintManager.engine;
	}

	/**
	 * @return The render constraint manager
	 */
	gp::graphics::ConstraintManager& renderConstraintManager()
	{
		return m_constraintManager.render;
	}

	/**
	 * @return The spot light
	 */
	gp::graphics::SpotLight& spotLight()
	{
		return m_spotLight;
	}

	/**
	 * @return The camera
	 */
	gp::graphics::Camera& camera()
	{
		return m_camera;
	}

	/**
	 * Synchronize the positions and rotation of engine and visualization components
	 */
	void synchronize();
};

}

#endif // GP_SCENARIO_H
