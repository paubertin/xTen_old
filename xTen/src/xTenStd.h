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
#include <list>
#include <vector>
#include <queue>
#include <map>

//

#include "maths\xmaths.h"

namespace xten {

#if defined(_DEBUG)
#	define XNEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define XNEW new
#endif

#define XDEL(p) if (p) { delete p; p = NULL; } 
#define XDEL_ARRAY(p) if (p) { delete[] p; p = NULL; }

	extern xmaths::vec3 g_X;
	extern xmaths::vec3 g_Y;
	extern xmaths::vec3 g_Z;

	extern xmaths::vec4 g_White;
	extern xmaths::vec4 g_Black;
	extern xmaths::vec4 g_Cyan;
	extern xmaths::vec4 g_Red;
	extern xmaths::vec4 g_Green;
	extern xmaths::vec4 g_Blue;
	extern xmaths::vec4 g_Yellow;
	extern xmaths::vec4 g_Gray40;
	extern xmaths::vec4 g_Gray25;
	extern xmaths::vec4 g_Gray65;
	extern xmaths::vec4 g_Transparent;

}