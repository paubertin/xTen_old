#include "window.h"
#include "../graphics/API/context.h"
#include "inputmanager.h"
#include "../xTenStd.h"

namespace xten {

	Window::Window(const std::string title, const WindowProperties & properties):
		m_Title(title), m_Properties(properties), m_ratio(1.0f)
	{
	}

	Window::~Window()
	{
	}

	void Window::clear()
	{
		GLCall( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	}

	void Window::onUpdate(GLint *w, GLint *h)
	{
		GLCall( glfwGetFramebufferSize(m_Window, &(m_Properties.width), &(m_Properties.height)) );
		*w = m_Properties.width;
		*h = m_Properties.height;

		GLCall( glfwSwapBuffers(m_Window) );
	}

}