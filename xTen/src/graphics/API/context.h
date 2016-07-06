#pragma once

#include "../../app/window.h"

namespace xten { namespace xgraphics { namespace API {

	enum class RenderAPI
	{
		NONE, OPENGL
	};

	enum XTEN_GL_FW_TYPE {
		XTEN_FW_TYPE_GLUT,
		XTEN_FW_TYPE_GLFW
	};

	enum XTEN_GL_PROFILE {
		XTEN_GL_ANY_PROFILE,
		XTEN_GL_COMPAT_PROFILE,
		XTEN_GL_CORE_PROFILE
	};
	
	class Context
	{
	protected:
		static Context* s_Context;
		static RenderAPI s_RenderAPI;
	public:
		static void Create(GLFWwindow* window);
	
		static RenderAPI getRenderAPI() { return s_RenderAPI; }
		static void setRenderAPI(RenderAPI api) { s_RenderAPI = api; }
	};

} } }