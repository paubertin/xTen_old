#pragma once

#include "object.h"

namespace xten {

	class Font : public Object
	{
	public:
		Font();
		virtual ~Font();
	public:
		enum FontFormat
		{
			BITMAP,
			DISTANCE_FIELD
		};
	};

}