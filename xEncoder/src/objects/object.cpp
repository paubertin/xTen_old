#include "object.h"

namespace xten {

	Object::Object()
	{ }

	Object::~Object()
	{ }

	unsigned int Object::getTypeID(void) const
	{
		return 0;
	}

	const std::string& Object::getID() const
	{
		return m_ID;
	}

	void Object::setID(const char* idStr)
	{
		if (idStr)
		{
			m_ID = idStr;
		}
	}

	void Object::setID(const std::string& newId)
	{
		if (newId.length() > 0)
		{
			m_ID = newId;
		}
	}

}