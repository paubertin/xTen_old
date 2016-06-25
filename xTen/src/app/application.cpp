#include "application.h"
#include "../utils/utils.h"
#include "../graphics/shaders/shadermanager.h"
#include "../graphics/texturemanager.h"


namespace xten {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
	}

	Application::~Application()
	{
		xgraphics::ShaderManager::clean();
		xgraphics::TexManager::clean();
		XDEL(m_Camera);
	}

	void Application::init()
	{
		std::cout << "Initializing xTen..." << std::endl;
		m_Camera = new xgraphics::Camera();
	}

	// TODO : get a general method (without GLFW)
	void Application::onUpdate(Window * window)
	{
		if (Input::isMouseButtonClicked(GLFW_MOUSE_BUTTON_LEFT))
		{
			if (m_Camera->isActive())
			{
				glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				m_Camera->setActive(GL_FALSE);
				m_Camera->init();
			}
			else
			{
				glfwSetInputMode(window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				glfwSetCursorPos(window->getGLFWWindow(), 0.5*window->getWidth(), 0.5*window->getHeight());
				m_Camera->setActive(GL_TRUE);
			}
		}
		m_Camera->update();
	}

}