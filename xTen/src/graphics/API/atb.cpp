#include "atb.h"
#include "../../xTenStd.h"

namespace xten { namespace xgraphics { namespace API {

	ATB::ATB()
	{}

	ATB::~ATB()
	{}

	bool ATB::init(XTEN_GL_PROFILE glprofile)
	{
		bool res = false;

		if (glprofile == XTEN_GL_CORE_PROFILE)
		{
			if (TwInit(TW_OPENGL_CORE, NULL) == 1)
			{
				res = true;
			}
		}
		else
		{
			if (TwInit(TW_OPENGL, NULL) == 1)
			{
				res = true;
			}
		}

		if (res)
		{
			XTEN_INFO("AntTweakBar initialized");
		}
		return res;
	}

} } }