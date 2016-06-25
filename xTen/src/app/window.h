#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>
#include <fstream>

namespace xten {

	struct WindowProperties
	{
		GLint width, height;
		GLboolean fullscreen;
		GLboolean vsync;
	};

	class Window
	{
	private:
		std::string m_Title;
		GLfloat m_ratio;
	public:
		Window(const std::string title, const WindowProperties & properties);
		GLFWwindow* m_Window;
		WindowProperties m_Properties;
		GLFWwindow* getGLFWWindow() { return m_Window; }

		~Window();
		void setTitle(std::string title) { glfwSetWindowTitle(m_Window, title.c_str()); }

		void clear();
		void onUpdate(GLint* w, GLint* h);
		void setRatio() { m_ratio = GLfloat(m_Properties.width) / GLfloat(m_Properties.height); }
		GLfloat getRatio() { return m_ratio; }
		const GLint getWidth() const { return m_Properties.width; }
		const GLint getHeight() const { return m_Properties.height; }
		const std::string getTitle() const { return m_Title; }

		inline GLboolean isFullScreen() const { return m_Properties.fullscreen; }
		inline GLboolean getVsync() const { return m_Properties.vsync; }
		inline GLboolean isClosed() const { return glfwWindowShouldClose(m_Window) == 1; }
	private:
	public:
	};

}