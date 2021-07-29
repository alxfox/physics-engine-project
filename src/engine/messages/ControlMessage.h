#ifndef GP_ENGINE_MESSAGES_CONTROLMESSAGE_H
#define GP_ENGINE_MESSAGES_CONTROLMESSAGE_H

#include "common/messages/Message.h"

namespace gp
{

namespace engine
{
/**
 * @brief Physics engine related messages
 */
namespace messages
{

/**
 * @brief Control the execution of the physics engine
 */
class ControlMessage : public gp::messages::Message
{
private:
  bool m_paused;

public:
	ControlMessage(bool paused)
		: gp::messages::Message(type_id<ControlMessage>()),
		m_paused(paused)
	{ }

	bool paused() const {
		return m_paused;
	}
};

/**
 * @brief Perform single simulation step
 */
class StepMessage : public gp::messages::Message
{
public:
	StepMessage() : gp::messages::Message(type_id<StepMessage>()) { }
};


class ShootMessage : public gp::messages::Message
{
private:
	Vector3f m_source;
	Vector3f m_direction;
	float_t m_intensity;

public:
	ShootMessage(Vector3f source, Vector3f direction,float_t intensity)
		: gp::messages::Message(type_id<ShootMessage>()),
		m_source(source),
		m_direction(direction),
		m_intensity(intensity)
	{ }

	Vector3f source() const {
		return m_source;
	}
	Vector3f direction() const {
		return m_direction;
	}
	float_t intensity() const {
		return m_intensity;
	}
};


class PlayerPositionMessage: public gp::messages::Message
{
private:
	Vector3f m_playerPosition;
  

public:
	PlayerPositionMessage(Vector3f playerPosition)
		: gp::messages::Message(type_id<PlayerPositionMessage>()),
		m_playerPosition(playerPosition)
	{ }

	Vector3f playerPosition() const {
		return m_playerPosition;
	}
};

class GravityMessage: public gp::messages::Message
{
private:
	Vector3f m_sceneAccel;
  

public:
	GravityMessage(Vector3f newAccel)
		: gp::messages::Message(type_id<GravityMessage>()),
		m_sceneAccel(newAccel)
	{ }

	Vector3f getSceneAccel() const {
		return m_sceneAccel;
	}
};

}

}

}

#endif // GP_ENGINE_MESSAGES_CONTROLMESSAGE_H
