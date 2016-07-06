#pragma once

#include "../utils/utils.h"

namespace xten {

	class Object
	{
	private:
		std::string m_ID;
	public:
		enum TypeID
		{
			FONT_ID
		};
	public:
		Object();
		virtual ~Object();
	public:
		virtual unsigned int getTypeID(void) const;
		const std::string& getID() const;
		void setID(const char* id);
		void setID(const std::string& id);

	};

}