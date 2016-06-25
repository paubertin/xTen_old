#include "string.h"

namespace xten {

	std::string ToStr(float num)
	{
		char str[64];
		memset(str, 0, 64);
		_sprintf_p(str, 64, "%f", num);
		return (std::string(str));
	}

}