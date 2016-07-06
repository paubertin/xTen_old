#include <GL\glew.h>

#include "GLcontext.h"
#include "../../xTenStd.h"

namespace xten { namespace xgraphics { namespace API {

	GLContext::GLContext(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE; //ensures GLEW uses more modern techniques for managing OpenGL functionality
		if (glewInit() != GLEW_OK)
		{
			XTEN_ERROR("[GLcontext]. Failed to initialize GLEW.");
		}

		std::string glversion = "OpenGL ";
		glversion += std::string((const char*)glGetString(GL_VERSION));
		XTEN_INFO(glversion);
		//std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	}

	void GLContext::Present()
	{
	}

} } }
