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

}