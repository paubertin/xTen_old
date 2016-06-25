#pragma once

#include "window.h"
#include <GLFW\glfw3.h>
#include "client.h"
#include "inputmanager.h"

namespace xten {
	
	bool GLFWInit();
	bool GLFWCreateWindow(Window * window);
	void GLFWTerminate(Window * window);
	void GLFWCallbacks(Window * window);
	void GLFWRun(Client * client);
	void glfw_error_callback(int error, const char* description);
	

}