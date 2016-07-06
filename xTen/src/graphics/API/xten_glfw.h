#pragma once

#include "../../app/window.h"
#include <GLFW\glfw3.h>
#include "../../app/client.h"
#include "../../app/inputmanager.h"
#include "context.h"
#include "../../utils/xtenkeys.h"

namespace xten { namespace xgraphics { namespace API {

	bool GLFWInit();
	bool GLFWCreateWindow(Window * window, XTEN_GL_PROFILE glprofile);
	void GLFWTerminate(Window * window);
	void GLFWCallbacks(Window * window);
	void GLFWRun(Client * client);
	void glfw_error_callback(int error, const char* description);

	static XTEN_KEY GLFWKeyToXTENKey(GLuint key)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE		:
				return XTEN_KEY_ESCAPE;
			case GLFW_KEY_F1			:
				return XTEN_KEY_F1;
			case GLFW_KEY_F2			:
				return XTEN_KEY_F2;
			case GLFW_KEY_F3			:
				return XTEN_KEY_F3;
			case GLFW_KEY_F4			:
				return XTEN_KEY_F4;
			case GLFW_KEY_F5			:
				return XTEN_KEY_F5;
			case GLFW_KEY_F6			:
				return XTEN_KEY_F6;
			case GLFW_KEY_F7			:
				return XTEN_KEY_F7;
			case GLFW_KEY_F8			:
				return XTEN_KEY_F8;
			case GLFW_KEY_F9			:
				return XTEN_KEY_F8;
			case GLFW_KEY_F10			:
				return XTEN_KEY_F10;
			case GLFW_KEY_F11			:
				return XTEN_KEY_F11;
			case GLFW_KEY_F12			:
				return XTEN_KEY_F12;

			case GLFW_KEY_GRAVE_ACCENT	:
				return XTEN_KEY_SQUARE;
			case GLFW_KEY_1				:
				return XTEN_KEY_1;
			case GLFW_KEY_2				:
				return XTEN_KEY_2;
			case GLFW_KEY_3				:
				return XTEN_KEY_3;
			case GLFW_KEY_4				:
				return XTEN_KEY_4;
			case GLFW_KEY_5				:
				return XTEN_KEY_5;
			case GLFW_KEY_6				:
				return XTEN_KEY_6;
			case GLFW_KEY_7				:
				return XTEN_KEY_7;
			case GLFW_KEY_8				:
				return XTEN_KEY_8;
			case GLFW_KEY_9				:
				return XTEN_KEY_9;
			case GLFW_KEY_0				:
				return XTEN_KEY_0;
			case GLFW_KEY_MINUS			:
				return XTEN_KEY_RIGHT_PARENTHESIS;
			case GLFW_KEY_EQUAL			:
				return XTEN_KEY_EQUAL;
			case GLFW_KEY_BACKSPACE		:
				return XTEN_KEY_BACKSPACE;
			case GLFW_KEY_TAB			:
				return XTEN_KEY_TAB;
			case GLFW_KEY_Q				:
				return XTEN_KEY_A;
			case GLFW_KEY_W				:
				return XTEN_KEY_Z;
			case GLFW_KEY_E				:
				return XTEN_KEY_E;
			case GLFW_KEY_R				:
				return XTEN_KEY_R;
			case GLFW_KEY_T				:
				return XTEN_KEY_T;
			case GLFW_KEY_Y				:
				return XTEN_KEY_Y;
			case GLFW_KEY_U				:
				return XTEN_KEY_U;
			case GLFW_KEY_I				:
				return XTEN_KEY_I;
			case GLFW_KEY_O				:
				return XTEN_KEY_O;
			case GLFW_KEY_P				:
				return XTEN_KEY_P;
			case GLFW_KEY_LEFT_BRACKET	:
				return XTEN_KEY_CIRCUMFLEX;
			case GLFW_KEY_RIGHT_BRACKET	:
				return XTEN_KEY_$;
			case GLFW_KEY_CAPS_LOCK		:
				return XTEN_KEY_CAPS_LOCK;
			case GLFW_KEY_A				:
				return XTEN_KEY_Q;
			case GLFW_KEY_S				:
				return XTEN_KEY_S;
			case GLFW_KEY_D				:
				return XTEN_KEY_D;
			case GLFW_KEY_F				:
				return XTEN_KEY_F;
			case GLFW_KEY_G				:
				return XTEN_KEY_G;
			case GLFW_KEY_H				:
				return XTEN_KEY_H;
			case GLFW_KEY_J				:
				return XTEN_KEY_J;
			case GLFW_KEY_K				:
				return XTEN_KEY_K;
			case GLFW_KEY_L				:
				return XTEN_KEY_L;
			case GLFW_KEY_SEMICOLON		:
				return XTEN_KEY_M;
			case GLFW_KEY_APOSTROPHE	:
				return XTEN_KEY_ù;
			case GLFW_KEY_BACKSLASH		:
				return XTEN_KEY_STAR;
			case GLFW_KEY_LEFT_SHIFT	:
				return XTEN_KEY_LEFT_SHIFT;
			case GLFW_KEY_WORLD_2		:
				return XTEN_KEY_LESS_THAN;
			case GLFW_KEY_Z				:
				return XTEN_KEY_W;
			case GLFW_KEY_X				:
				return XTEN_KEY_X;
			case GLFW_KEY_C				:
				return XTEN_KEY_C;
			case GLFW_KEY_V				:
				return XTEN_KEY_V;
			case GLFW_KEY_B				:
				return XTEN_KEY_B;
			case GLFW_KEY_N				:
				return XTEN_KEY_N;
			case GLFW_KEY_M				:
				return XTEN_KEY_COMMA;
			case GLFW_KEY_COMMA			:
				return XTEN_KEY_SEMICOLON;
			case GLFW_KEY_PERIOD		:
				return XTEN_KEY_COLON;
			case GLFW_KEY_SLASH			:
				return XTEN_KEY_EXCLAMATION;
			case GLFW_KEY_RIGHT_SHIFT	:
				return XTEN_KEY_RIGHT_SHIFT;
			case GLFW_KEY_LEFT_CONTROL	:
				return XTEN_KEY_LEFT_CONTROL;
			case GLFW_KEY_LEFT_SUPER	:
				return XTEN_KEY_WINDOWS;
			case GLFW_KEY_LEFT_ALT		:
				return XTEN_KEY_LEFT_ALT;
			case GLFW_KEY_SPACE			:
				return XTEN_KEY_SPACE;
			case GLFW_KEY_RIGHT_ALT		:
				return XTEN_KEY_RIGHT_ALT;

			case GLFW_KEY_MENU			:
				return XTEN_KEY_MENU;
			case GLFW_KEY_RIGHT_CONTROL	:
				return XTEN_KEY_RIGHT_CONTROL;
			case GLFW_KEY_ENTER			:
				return XTEN_KEY_ENTER;

			case GLFW_KEY_SCROLL_LOCK	:
				return XTEN_KEY_SCROLL_LOCK;
			case GLFW_KEY_PAUSE			:
				return XTEN_KEY_PAUSE;
			case GLFW_KEY_INSERT		:
				return XTEN_KEY_INSERT;
			case GLFW_KEY_HOME			:
				return XTEN_KEY_HOME;
			case GLFW_KEY_PAGE_UP		:
				return XTEN_KEY_PAGE_UP;
			case GLFW_KEY_DELETE		:
				return XTEN_KEY_DELETE;
			case GLFW_KEY_END			:
				return XTEN_KEY_END;
			case GLFW_KEY_PAGE_DOWN		:
				return XTEN_KEY_PAGE_DOWN;

			case GLFW_KEY_UP			:
				return XTEN_KEY_UP;
			case GLFW_KEY_LEFT			:
				return XTEN_KEY_LEFT;
			case GLFW_KEY_DOWN			:
				return XTEN_KEY_DOWN;
			case GLFW_KEY_RIGHT			:
				return XTEN_KEY_RIGHT;

			case GLFW_KEY_NUM_LOCK		:
				return XTEN_KEY_NUM_LOCK;
			case GLFW_KEY_KP_DIVIDE		:
				return XTEN_KEY_KP_DIVIDE;
			case GLFW_KEY_KP_MULTIPLY	:
				return XTEN_KEY_KP_MULTIPLY;
			case GLFW_KEY_KP_SUBTRACT	:
				return XTEN_KEY_KP_SUBTRACT;
			case GLFW_KEY_KP_7			:
				return XTEN_KEY_KP_7;
			case GLFW_KEY_KP_8			:
				return XTEN_KEY_KP_8;
			case GLFW_KEY_KP_9			:
				return XTEN_KEY_KP_9;
			case GLFW_KEY_KP_4			:
				return XTEN_KEY_KP_4;
			case GLFW_KEY_KP_5			:
				return XTEN_KEY_KP_5;
			case GLFW_KEY_KP_6			:
				return XTEN_KEY_KP_6;
			case GLFW_KEY_KP_1			:
				return XTEN_KEY_KP_1;
			case GLFW_KEY_KP_2			:
				return XTEN_KEY_KP_2;
			case GLFW_KEY_KP_3			:
				return XTEN_KEY_KP_3;
			case GLFW_KEY_KP_0			:
				return XTEN_KEY_KP_0;
			case GLFW_KEY_KP_DECIMAL	:
				return XTEN_KEY_KP_DECIMAL;
			case GLFW_KEY_KP_ADD		:
				return XTEN_KEY_KP_ADD;
			case GLFW_KEY_KP_ENTER		:
				return XTEN_KEY_KP_ENTER;

			case GLFW_KEY_UNKNOWN		:
				return XTEN_KEY_UNDEFINED;
		}

		return XTEN_KEY_UNDEFINED;
	}

} } }