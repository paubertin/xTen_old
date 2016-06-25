#pragma once

#include <GL\glew.h>

#include <string>
#include "../maths/xmaths.h"

namespace xten { namespace xgraphics {

	using namespace xmaths;

	struct BaseLight
	{
		std::string m_Name;
		vec3 m_Color;
		GLfloat m_AmbientIntensity;
		GLfloat m_DiffuseIntensity;

		BaseLight() :
			m_Color(vec3(0.0f)), m_AmbientIntensity(0.0f), m_DiffuseIntensity(0.0f)
		{
		}
	};

	struct DirectionalLight : public BaseLight
	{
		DirectionalLight():
			BaseLight(), m_Direction(vec3(1.0f))
		{ }

		vec3 m_Direction;
	};

	struct PointLight : public BaseLight
	{
		PointLight():
			BaseLight(), m_Position(vec3(0.0f))
		{
			m_Attenuation.Const = 1.0f;
			m_Attenuation.Lin = 0.0f;
			m_Attenuation.Quad = 0.0f;
		}

		struct
		{
			float Const;
			float Lin;
			float Quad;
		} m_Attenuation;

		vec3 m_Position;

	};

	struct SpotLight : public PointLight
	{
		SpotLight() :
			PointLight(), m_Direction(vec3(0.0f)), m_Cutoff(0.0f)
		{
		}

		vec3 m_Direction;
		float m_Cutoff;

	};

} }