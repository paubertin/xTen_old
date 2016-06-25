#pragma once

#include "../graphics/camera/camera.h"
#include "xvec.h"
#include "xmat.h"

namespace xten { namespace xmaths {

	struct PerspProjData
	{
		float fov;
		float aspectRatio;
		float near;
		float far;
	};

	class Pipeline
	{
	private:
		vec3 m_Scale;
		vec3 m_WorldPos;
		float m_Angle;
		vec3 m_Axis;
		mat4 m_Model;

		PerspProjData m_PerspProjData;
		mat4 m_Proj;

		struct {
			vec3 pos;
			vec3 target;
			vec3 up;
		} m_camera;
		mat4 m_View;
	public:
		Pipeline();
		~Pipeline();

		const mat4& getModel();
		const mat4& getPerspProj();
		const mat4& getView();

		void scale(float s);
		void scale(float x, float y, float z);
		void scale(const vec3 &vec);
		void worldPos(float x, float y, float z);
		void worldPos(const vec3 &pos);
		void rotate(float angle, float x, float y, float z);
		void rotate(float angle, const vec3 &vec);

		void setPerspProj(float fov, float aspectRatio, float near, float far);
		void setPerspProj(const PerspProjData & pdata);
		
		void setCamera(const xgraphics::Camera& cam);
	};

} }