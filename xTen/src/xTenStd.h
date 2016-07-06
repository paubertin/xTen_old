#pragma once

#ifdef _WIN32 || _WIN64

// Windows Header Files:

#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#undef near
#undef far

#include <crtdbg.h>

#define _VS2005_
#if _MSC_VER < 1400
#undef _VS2005_
#endif

#if _MSC_VER >= 1700  // VS 2012 or later
#include <memory>
using std::shared_ptr;
using std::weak_ptr;
using std::unique_ptr;
using std::static_pointer_cast;
using std::dynamic_pointer_cast;
#elif _MSC_VER == 1600  // VS 2010
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
#endif

#endif // _WIN32 || _WIN64

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

//#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <fstream>
#include <cstddef>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>

//
#include <GL\glew.h>
#include "ext\tinyxml\tinyxml.h"

#include "system\logger.h"
#include "maths\xmaths.h"
#include "utils\string.h"

namespace xten {

	class xNonCopyable
	{
	private:
		xNonCopyable(const xNonCopyable& x);
		xNonCopyable& operator=(const xNonCopyable& x);
	public:
		xNonCopyable() {};
	};

#if defined(_DEBUG)
#define XNEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#define XASSERT(expression) assert(expression)
#else
#define XNEW new
#define XASSERT(expression)
#endif

#define XDEL(p) if (p) { delete p; p = NULL; } 
#define XDEL_ARRAY(p) if (p) { delete[] p; p = NULL; }
#define XDEL_VEC(p) for (unsigned int i=0; i < p.size(); i++) { XDEL(p[i]); }

	extern xten::xmaths::vec3 g_X;
	extern xten::xmaths::vec3 g_Y;
	extern xten::xmaths::vec3 g_Z;

	extern xten::xmaths::vec4 g_White;
	extern xten::xmaths::vec4 g_Black;
	extern xten::xmaths::vec4 g_Cyan;
	extern xten::xmaths::vec4 g_Red;
	extern xten::xmaths::vec4 g_Green;
	extern xten::xmaths::vec4 g_Blue;
	extern xten::xmaths::vec4 g_Yellow;
	extern xten::xmaths::vec4 g_Gray40;
	extern xten::xmaths::vec4 g_Gray25;
	extern xten::xmaths::vec4 g_Gray65;
	extern xten::xmaths::vec4 g_Transparent;

	//////// GL Errors - Log

#ifdef _DEBUG
#define GLCall(glcode) glcode; __gl_error_ = glGetError(); XTEN_ASSERT(__gl_error_ == GL_NO_ERROR); 
#else
#define GLCall(glcode) glcode;
#endif

	extern GLenum __gl_error_;

}