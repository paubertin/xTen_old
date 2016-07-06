#include "xTenStd.h"

namespace xten {

	xten::xmaths::vec3 g_X(1.0f, 0.0f, 0.0f); // right
	xten::xmaths::vec3 g_Y(0.0f, 1.0f, 0.0f); // up
	xten::xmaths::vec3 g_Z(0.0f, 0.0f, 1.0f); // screen to player

	xten::xmaths::vec4 g_White(1.0f, 1.0f, 1.0f, 1.0f);
	xten::xmaths::vec4 g_Black(0.0f, 0.0f, 0.0f, 1.0f);
	xten::xmaths::vec4 g_Cyan(0.0f, 1.0f, 1.0f, 1.0f);
	xten::xmaths::vec4 g_Red(1.0f, 0.0f, 0.0f, 1.0f);
	xten::xmaths::vec4 g_Green(0.0f, 1.0f, 0.0f, 1.0f);
	xten::xmaths::vec4 g_Blue(0.0f, 0.0f, 1.0f, 1.0f);
	xten::xmaths::vec4 g_Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	xten::xmaths::vec4 g_Gray40(0.4f, 0.4f, 0.4f, 1.0f);
	xten::xmaths::vec4 g_Gray25(0.25f, 0.25f, 0.25f, 1.0f);
	xten::xmaths::vec4 g_Gray65(0.65f, 0.65f, 0.65f, 1.0f);
	xten::xmaths::vec4 g_Transparent(1.0f, 1.0f, 1.0f, 0.0f);

	GLenum __gl_error_ = GL_NO_ERROR;

}