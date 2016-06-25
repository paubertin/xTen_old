#pragma once

#include "../../app/window.h"

namespace xten { namespace xgraphics { namespace API {

	enum class RenderAPI
	{
		NONE, OPENGL
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