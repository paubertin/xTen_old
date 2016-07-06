#pragma once

#include <AntTweakBar.h>
#include "context.h"

namespace xten { namespace xgraphics { namespace API {

	class ATB
	{
	public:
		ATB();
		~ATB();
	public:
		bool init(XTEN_GL_PROFILE glprofile);

	};

} } }