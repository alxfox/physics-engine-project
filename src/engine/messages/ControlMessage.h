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

}

}

}

#endif // GP_ENGINE_MESSAGES_CONTROLMESSAGE_H
