#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "../maths/xmaths.h"
#include "../utils/xtenkeys.h"


#define MAX_KEYS		1024
#define MAX_BUTTONS		32

namespace xten {

	class InputManager
	{
	public:
		InputManager();
		~InputManager();
	private:
		GLboolean m_Keys[MAX_KEYS];
		GLboolean m_KeyState[MAX_KEYS];
		GLboolean m_KeyTyped[MAX_KEYS];

		GLboolean m_MouseButtons[MAX_BUTTONS];
		GLboolean m_MouseState[MAX_BUTTONS];
		GLboolean m_MouseClicked[MAX_BUTTONS];

		GLfloat m_MouseWheelXoffset;
		GLfloat m_MouseWheelYoffset;

		xmaths::vec2 m_MousePosition;
		xmaths::vec2 m_MouseLastPosition;

	public:
		void onUpdate();

		inline void setKey(GLuint keycode, GLboolean state) { m_Keys[keycode] = state; }

		GLboolean getKey(GLuint keycode) const
		{
			if (keycode > MAX_KEYS)
			{
				return false;
			}
			return m_Keys[keycode];
		}
		GLboolean getKeyState(GLuint keycode) const
		{
			if (keycode > MAX_KEYS)
			{
				return false;
			}
			return m_KeyState[keycode];
		}
		GLboolean getKeyTyped(GLuint keycode) const
		{
			if (keycode > MAX_KEYS)
			{
				return false;
			}
			return m_KeyTyped[keycode];
		}

		inline void setMouseButton(GLuint button, GLboolean state)
		{
			m_MouseButtons[button] = state;
		}

		GLboolean getMouseButton(GLuint button) const
		{
			if (button > MAX_BUTTONS)
			{
				return false;
			}
			return m_MouseButtons[button];
		}
		GLboolean getMouseState(GLuint button) const
		{
			if (button > MAX_BUTTONS)
			{
				return false;
			}
			return m_MouseState[button];
		}
		GLboolean getMouseClicked(GLuint button) const
		{
			if (button > MAX_BUTTONS)
			{
				return false;
			}
			return m_MouseClicked[button];
		}

		inline void incrementMouseWheelXoffset(GLfloat xoffset)
		{
			m_MouseWheelXoffset += xoffset;
		}

		inline void incrementMouseWheelYoffset(GLfloat yoffset)
		{
			m_MouseWheelYoffset += yoffset;
		}

		GLfloat getMouseWheelXoffset() const { return m_MouseWheelXoffset; }
		GLfloat getMouseWheelYoffset() const { return m_MouseWheelYoffset; }

		inline void setCursorPosition(GLfloat xpos, GLfloat ypos)
		{
			m_MousePosition.x = xpos;
			m_MousePosition.y = ypos;
		}

		const xmaths::vec2& getMousePosition() const { return m_MousePosition; }
		const xmaths::vec2& getMouseLastPosition() const { return m_MouseLastPosition; }

		inline void resetMousePosition() { m_MouseLastPosition = m_MousePosition; }
		inline void resetWheelY() { m_MouseWheelYoffset = 0.f; };
		inline void resetWheelX() { m_MouseWheelXoffset = 0.f; };

	};

	class Input
	{
		friend class InputManager;
	private:
		static InputManager* s_InputManager;
	public:
		inline static InputManager* getInputManager() { return s_InputManager; }

		inline static GLboolean isKeyPressed(GLuint keycode) { return s_InputManager->getKey(keycode); }
		inline static GLboolean isKeyTyped(GLuint keycode) { return s_InputManager->getKeyTyped(keycode); }
		inline static GLboolean isMouseButtonPressed(GLuint button) { return s_InputManager->getMouseButton(button); }
		inline static GLboolean isMouseButtonClicked(GLuint button) { return s_InputManager->getMouseClicked(button); }

		inline static const xmaths::vec2& getMousePosition() { return s_InputManager->getMousePosition(); }
		inline static const xmaths::vec2& getMouseLastPosition() { return s_InputManager->getMouseLastPosition(); }

	};
}