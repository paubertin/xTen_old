#pragma once

#include "../maths/xmaths.h"

namespace xten { namespace xgraphics {

	using namespace xmaths;

	struct Vertex
	{
		vec3 pos;
		vec2 uv;
		vec3 normal;

		Vertex() {}

		Vertex(const vec3 &position, const vec2 &texture)
		{
			pos = position;
			uv = texture;
			normal = vec3(0.f);
		}

		Vertex(const vec3 &position, const vec2& texture, const vec3 &nor)
		{
			pos = position;
			uv = texture;
			normal = nor;
		}

	};

} }