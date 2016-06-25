#include "context.h"
#include "GLcontext.h"
#include "../../xTenStd.h"

namespace xten { namespace xgraphics { namespace API {

	Context* Context::s_Context = nullptr;
	RenderAPI Context::s_RenderAPI = RenderAPI::NONE;
	
	void Context::Create(GLFWwindow* window)
	{
		switch (getRenderAPI())
		{
			case RenderAPI::OPENGL:		s_Context = XNEW GLContext(window); break;
		}
	}

} } }