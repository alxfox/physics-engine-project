#ifndef GP_COMMON_MESSAGES_STOPMESSAGE_H
#define GP_COMMON_MESSAGES_STOPMESSAGE_H

#include "Message.h"

namespace gp
{

namespace messages
{

/**
 * Execution should be stopped when this message is reached
 */
class StopMessage : public Message
{
public:
	StopMessage() : Message(type_id<StopMessage>())
	{ }
};

}

}

#endif // GP_COMMON_MESSAGES_STOPMESSAGE_H