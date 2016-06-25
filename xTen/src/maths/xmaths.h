#pragma once

#include "xrandom.h"
#include "primesearch.h"
#include "xvec.h"
#include "xmat.h"
#include "pipeline.h"

namespace xten { namespace xmaths {

#ifndef xPI
#define xPI 3.14159265358979f
#endif // !xPI



	template<typename T>
	inline T deg2rad(T deg)
	{
		return (T(deg * xPI / 180));
	}

	template<typename T>
	inline T rad2deg(T rad)
	{
		return (T(rad * 180 / xPI));
	}

} }