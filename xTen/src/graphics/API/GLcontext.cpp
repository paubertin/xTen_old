#include <GL\glew.h>

#include "GLcontext.h"

namespace xten { namespace xgraphics { namespace API {

	GLContext::GLContext(GLFWwindow* window)
	{
		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE; //ensures GLEW uses more modern techniques for managing OpenGL functionality
		if (glewInit() != GLEW_OK)
		{
			std::cout << "ERROR [GLcontext]. Failed to initialize GLEW." << std::endl
				<< "-- --------------------------------- --" << std::endl;
		}

		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	}

} } }
