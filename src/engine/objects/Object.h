#ifndef GP_ENGINE_OBJECTS_OBJECT_H
#define GP_ENGINE_OBJECTS_OBJECT_H

#include <string>

#include "engine/utils.h"
#include "engine/math/Quaternion.h"

namespace gp
{

namespace engine
{

/**
 * @brief Base class for all objects in the physics engine
 */
class Object
{
private:
	/** Name of the object (can be used for debugging) */
	std::string m_name;



	/** 1/mass of the object */
	float_t m_invMass;

	/** The current position (center) of the object */
	Vector3f m_position;

	/** The current rotation of the object */
	Quaternion m_rotation;

	/** The current velocity of the object */
	Vector3f m_velocity;

	/** The angular velocity of the object in world space */
	Vector3f m_angularVelocity;

	/** The model matrix of the object (as a transformation object) */
	Affine3f m_modelMatrix;

	/** The inverse model matrix of the object (as a transformation object) */
	Affine3f m_inverseModelMatrix;

	/**
	 * Coefficient of restitution (COF) controls the amount of separating velocity depending
	 * on the closing velocity
	 *
	 * the COF for 2 objects is computed as the minimum COF of both
	 */
	float_t m_restitutionCoefficient;

	/** Static friction coefficient */
	float_t m_staticFriction;

	/** Dynamic friction coefficient */
	float_t m_dynamicFriction;

	/** The rotational inertia (stored as inverse) */
	Matrix3f m_rotationalInverseInertia;

	/** Bounding radius for quick collision detection (an upper bound of the size) */
	const float_t m_boundingRadius;

	float_t m_distToCam; 

	int m_shotsReceived;

protected:

public:

	enum ObjectType 
	{
		DEFAULT,
		DEFAULT_ENEMY,
		DEFAULT_POWER_UP,
		TRIGGER_PLAYER,
		TRIGGER_ZONE_0,
		TRIGGER_ZONE_1,
		CUBE
	};	
	/** The object is not a rigid body but a trigger **/
	ObjectType m_type;

	bool isCube;

	bool m_destructible = true;
	bool isTriggered = false;
	Object(float_t mass, const Vector3f &position, const Vector3f &velocity, const Quaternion &rotation, const Matrix3f &rotationalInertia, float_t boundingRadius, ObjectType type = DEFAULT)
		: m_invMass(1./mass), m_position(position), m_rotation(rotation), m_velocity(velocity),
		m_angularVelocity(Vector3f::Zero()),
		m_restitutionCoefficient(0.3),
		m_staticFriction(0.4),
		m_dynamicFriction(0.3),
		m_rotationalInverseInertia(rotationalInertia.inverse()),
		m_boundingRadius(boundingRadius),
		m_type(type),
		m_distToCam(10),
		m_shotsReceived(0)
	{
		updateModelMatrix();
	}

	virtual ~Object()
	{ }

	float_t distToCam() const
	{
		return m_distToCam;
	}

	void setDistToCam(float_t dToCam)
	{
		m_distToCam = dToCam;
	}
	/**
	 * @return The name of the object or an empty string if no name was set
	 */
	const std::string& name() const
	{
		return m_name;
	}

	/**
	 * @brief Sets the object name
	 *
	 * This function is especially useful for debugging
	 */
	void setName(const std::string &name)
	{
		m_name = name;
	}

	/**
	 * @return The inverted (1./mass) mass
	 */
	float_t invMass() const
	{
		return m_invMass;
	}
	
	/**
	 * @return The mass of the object
	 */
	float_t mass() const
	{
		return 1./m_invMass;
	}

	void setMass(float_t newMass){
		m_invMass = 1./newMass;
		updateModelMatrix();
	}

	void setShots(int newShowts){
		m_shotsReceived = newShowts;
	}

	void receiveShot(){
		m_shotsReceived += 1;
	}

	int numShots(){
		return m_shotsReceived;
	}

	/**
	 * @return True, if the object can be moved
	 */
	bool isMovable() const
	{
		return m_invMass != INV_UNMOVABLE_MASS;
	}

	/**
	 * @return True, if the object is a trigger 
	 */
	ObjectType objType() const
	{
		return m_type;
	}

	/**
	 * @return The current position of the object
	 */
	Vector3f position() const
	{
		return m_position;
	}

	void setPosition(const Vector3f &position)
	{
		m_position = position;
		updateModelMatrix();
	}

	/**
	 * Move the object along a given vector
	 */
	void move(const Vector3f &distance)
	{
		m_position += distance;
		updateModelMatrix();
	}

	/**
	 * @return The current rotation of the object
	 */
	Quaternion rotation() const
	{
		return m_rotation;
	}

	void setRotation(const Quaternion &rotation)
	{
		m_rotation = rotation;
	}

	/**
	 * @return The current velocity of the object
	 */
	Vector3f velocity() const
	{
		return m_velocity;
	}

	void setVelocity(const Vector3f &velocity)
	{
		m_velocity = velocity;
	}

	/**
	 * Changes the velocity by adding the given velocity difference
	 */
	void changeVelocity(const Vector3f &velocityDiff)
	{
		m_velocity += velocityDiff;
	}

	/**
	 * Does one step by updating the velocity of the object
	 */
	void updateVelocity(const Vector3f &acceleration, float_t step)
	{
		m_velocity += (step * acceleration); //Stoermer Verlet (a(t) and a(t+delta t)) are equal, making this the same as the Euler Formula 
		// long version m_velocity += (step * (acceleration + acceleration))/2;
	}

	/**
	 * @return The current angular velocity of the object
	 */
	Vector3f angularVelocity() const
	{
		return m_angularVelocity;
	}

	void setAngularVelocity(const Vector3f &angularVelocity)
	{
		m_angularVelocity = angularVelocity;
	}

	/**
	 * Change the angular velocity by adding the given difference
	 */
	void changeAngularVelocity(const Vector3f &velocityDiff)
	{
		m_angularVelocity += velocityDiff;
	}

	/**
	 * Does one step for the object by updating the position and rotation
	 * from the velocity and angular velocity.																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																								
	 */
	void updatePositionAndRotation(const Vector3f &acceleration, float_t step)
	{
		// Update the position
		m_position += step * m_velocity + ((acceleration * step * step) / 2); // Stroemer Verlet (respects conservation of energy)

		// Update the rotation
		//m_rotation.rotate(step * m_angularVelocity);
		float_t i = (step/2.f) * m_angularVelocity(0);
		float_t j = (step/2.f) * m_angularVelocity(1);
		float_t k = (step/2.f) * m_angularVelocity(2);
		Quaternion w{i,j,k,0};
		m_rotation = m_rotation + (w * m_rotation);
		// Cache the model matrix
		updateModelMatrix();
	}

	float_t restitutionCoefficient() const
	{
		return m_restitutionCoefficient;
	}

	void setRestitutionCoefficient(float_t restitutionCoefficient)
	{
		m_restitutionCoefficient = restitutionCoefficient;
	}

	float_t staticFrictionCoefficient() const
	{
		return m_staticFriction;
	}

	void setStaticFrictionCoefficient(float_t staticFriction)
	{
		m_staticFriction = staticFriction;
	}

	float_t dynamicFrictionCoefficient() const
	{
		return m_dynamicFriction;
	}

	void setDynamicFrictionCoefficient(float_t dynamicFriction)
	{
		m_dynamicFriction = dynamicFriction;
	}

	const Matrix3f& rotationalInverseInertia() const
	{
		return m_rotationalInverseInertia;
	}

	float_t boundingRadius() const
	{
		return m_boundingRadius;
	}

	const Affine3f modelMatrix() const
	{
		return m_modelMatrix;
	}

	const Affine3f invModelMatrix() const
	{
		return m_inverseModelMatrix;
	}

private:
	void updateModelMatrix()
	{
		m_rotation.normalize(); // Prevents instabilities
		m_modelMatrix = Eigen::Translation<float_t, 3>(m_position) * m_rotation.rotationMatrix();
		m_inverseModelMatrix = m_modelMatrix.inverse();
	}

public:
	/** Objects with this mass will never move (but collide with other objects) */
	static constexpr float_t UNMOVABLE_MASS = std::numeric_limits<float_t>::infinity();

private:
	/** The inverse of {@link UNMOVABLE_MASS} for fast comparision */
	static constexpr float_t INV_UNMOVABLE_MASS = 1./UNMOVABLE_MASS;
};

}

}

#endif // GP_ENGINE_OBJECTS_OBJECT_H
