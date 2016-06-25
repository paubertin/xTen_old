#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "../maths/xmaths.h"


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

		GLdouble m_MouseWheelXoffset;
		GLdouble m_MouseWheelYoffset;

		xmaths::vec2 m_MousePosition;
		xmaths::vec2 m_MouseLastPosition;
		friend void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
		friend void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
		friend void cursor_position_callback(GLFWwindow * window, double xpos, double ypos);
		friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	public:
		void onUpdate();

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

		GLdouble getMouseWheelXoffset() const { return m_MouseWheelXoffset; }
		GLdouble getMouseWheelYoffset() const { return m_MouseWheelYoffset; }

		const xmaths::vec2& getMousePosition() const { return m_MousePosition; }
		const xmaths::vec2& getMouseLastPosition() const { return m_MouseLastPosition; }

		void resetMousePosition();
		void resetWheelY() { m_MouseWheelYoffset = 0.; };
		void setCallbacks(GLFWwindow *window);
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