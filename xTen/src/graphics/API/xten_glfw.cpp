#include <fstream>
#include <iostream>

#include "xten_glfw.h"

namespace xten { namespace xgraphics { namespace API {

	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "GLFW Error: %s\n", description);
	}

	static void framebuffer_size_callback(GLFWwindow* window, GLint width, GLint height)
	{
		GLCall( glViewport(0, 0, width, height) );
	}

	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		XTEN_KEY xkey = GLFWKeyToXTENKey(key);
		imanager->setKey(xkey, (action != GLFW_RELEASE));
	}
	
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		imanager->setMouseButton(button, (action != GLFW_RELEASE));
	}
	
	static void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		imanager->setCursorPosition((GLfloat)(xpos), (GLfloat)(ypos));
	}
	
	static void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		if (yoffset != 0.)
		{
			imanager->incrementMouseWheelYoffset((GLfloat)(yoffset));
		}
		if (xoffset != 0.)
		{
			imanager->incrementMouseWheelXoffset((GLfloat)(xoffset));
		}
	}

	bool GLFWInit()
	{
		glfwSetErrorCallback(glfw_error_callback);

		if (!glfwInit())
		{
			XTEN_ERROR("[GLFW]. Failed to initialize GLFW.");
			return GL_FALSE;
		}

		int Major, Minor, Rev;
		glfwGetVersion(&Major, &Minor, &Rev);
		printf("GLFW %d.%d.%d initialized\n", Major, Minor, Rev);

		return true;
	}

	bool GLFWCreateWindow(Window * window, XTEN_GL_PROFILE glprofile)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

		switch (glprofile)
		{
		case XTEN_GL_ANY_PROFILE:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE must be used
			break;
		case XTEN_GL_COMPAT_PROFILE:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
			break;
		case XTEN_GL_CORE_PROFILE:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //result in invalid operation errors whenever we call one of OpenGL's legacy functions
			break;
		default:
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE must be used
			break;
		}

		GLFWmonitor* pMonitor = window->isFullScreen() ? glfwGetPrimaryMonitor() : NULL;

		window->m_Window = glfwCreateWindow(window->getWidth(), window->getHeight(), (window->getTitle()).c_str(), pMonitor, NULL);
		if (!window->m_Window)
		{
			XTEN_ERROR("[GLFW]. Failed to create GLFW window.");
			return false;
		}

		glfwMakeContextCurrent(window->m_Window);

		glewExperimental = GL_TRUE; //ensures GLEW uses more modern techniques for managing OpenGL functionality

		GLenum res = glewInit();
		if (res != GLEW_OK)
		{
			std::string glewerror = "[GLEW]. ";
			glewerror += std::string((const char*)glewGetErrorString(res));
			XTEN_ERROR(glewerror);
			//std::cout << "ERROR [GLEW]. " << (const char*)glewGetErrorString(res) << std::endl
			//	<< "-- --------------------------------- --" << std::endl;
			return false;
		}

		if (!window->getVsync())
		{
			glfwSwapInterval(0);
		}

		return true;

	}

	void GLFWTerminate(Window * window)
	{
		glfwDestroyWindow(window->m_Window);
		glfwTerminate();
	}

	void GLFWCallbacks(Window * window)
	{
		GLFWwindow *win = window->m_Window;

		glfwSetWindowUserPointer(win, window);
		glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);

		glfwSetWindowUserPointer(win, Input::getInputManager());
		//Input::getInputManager()->setCallbacks(win);
		glfwSetKeyCallback(win, key_callback);
		glfwSetMouseButtonCallback(win, mouse_button_callback);
		glfwSetCursorPosCallback(win, cursor_position_callback);
		glfwSetScrollCallback(win, scroll_callback);
	}

	void GLFWRun(Client * client)
	{
		while (client->isRunning())
		{
			client->onUpdate();
			client->onRender();

			Window *win = client->getWindow();

			GLCall( glfwGetFramebufferSize(win->getGLFWWindow(), &win->m_Properties.width, &win->m_Properties.height) );
			win->setRatio();
			GLCall( glfwSwapBuffers(client->getWindow()->getGLFWWindow()) );
			GLCall( glfwPollEvents() );

			if (glfwWindowShouldClose(win->getGLFWWindow()))
			{
				client->close();
			}
		}
	}


} } }