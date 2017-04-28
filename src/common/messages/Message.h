#ifndef GP_COMMON_MESSAGES_MESSAGE_H
#define GP_COMMON_MESSAGES_MESSAGE_H

#include "common/utils/typeid.h"

namespace gp
{

namespace messages
{

/**
 * @brief Base class for messages in the message queues
 */
class Message
{
private:
	/** The message type */
	typeid_t m_type;

protected:
	/**
	 * @param type The type of the message. Set this to <code>type_id<Subclass>()</code>
	 *  from all subclasses.
	 */
	Message(typeid_t type)
		: m_type(type)
	{ }

public:
	virtual ~Message()
	{ }

	/**
	 * @return The type id of the message
	 */
	typeid_t type() const
	{
		return m_type;
	}
};

/**
 * @return The static type id of a message class
 */
template<class T>
typeid_t type()
{
	return type_id<T>();
}

/**
 * @return <code>true</code> if the message type matches the template type
 */
template<class T>
bool isType(Message m)
{
	return type<T>() == m.type();
}

}

}

#endif // GP_COMMON_MESSAGES_MESSAGE_H