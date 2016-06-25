#include "inputmanager.h"
#include <iostream>

namespace xten {

	InputManager* Input::s_InputManager = nullptr;

	InputManager::InputManager()
	{
		for (GLint i = 0; i < MAX_KEYS; ++i)
		{
			m_Keys[i] = false;
			m_KeyState[i] = false;
			m_KeyTyped[i] = false;
		}
		for (GLint i = 0; i < MAX_BUTTONS; ++i)
		{
			m_MouseButtons[i] = false;
			m_MouseState[i] = false;
			m_MouseClicked[i] = false;
		}

		m_MouseLastPosition = xmaths::vec2(0.f);
		m_MouseWheelYoffset = 0.;

		Input::s_InputManager = this;

		//m_MouseLastPosition = initpos;
	}

	InputManager::~InputManager()
	{

	}


	void InputManager::onUpdate()
	{
		for (GLint i = 0; i < MAX_KEYS; i++)
			m_KeyTyped[i] = m_Keys[i] && !m_KeyState[i];
		memcpy(m_KeyState, m_Keys, MAX_KEYS); // * sizeof(bool) (=1)

		for (GLint i = 0; i < MAX_BUTTONS; i++)
			m_MouseClicked[i] = m_MouseButtons[i] && !m_MouseState[i];
		memcpy(m_MouseState, m_MouseButtons, MAX_BUTTONS);
	}

	void InputManager::resetMousePosition()
	{
		m_MouseLastPosition = m_MousePosition;
	}

	void InputManager::setCallbacks(GLFWwindow *window)
	{
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetScrollCallback(window, scroll_callback);
	}

	void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		imanager->m_Keys[key] = (action != GLFW_RELEASE);
	}

	void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		imanager->m_MouseButtons[button] = (action != GLFW_RELEASE);
	}

	void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		imanager->m_MousePosition.x = (float)xpos;
		imanager->m_MousePosition.y = (float)ypos;
	}

	void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
	{
		InputManager *imanager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
		if (yoffset != 0.)
		{
			imanager->m_MouseWheelYoffset += yoffset;
		}
		if (xoffset != 0.)
		{
			imanager->m_MouseWheelXoffset += xoffset;
		}
	}
}