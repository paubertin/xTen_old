#pragma once

#include "context.h"

namespace xten { namespace xgraphics { namespace API {

	class GLContext : public Context
	{
	public:
		GLContext(GLFWwindow* window);
	
		void Present();
	public:
		inline static GLContext* get() { return (GLContext*)s_Context; }
	};

} } }