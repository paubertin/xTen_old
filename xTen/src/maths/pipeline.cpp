#include "pipeline.h"

namespace xten { namespace xmaths {

	Pipeline::Pipeline():
		m_Scale(vec3(1.f)), m_WorldPos(vec3(0.f)), m_Angle(0.f), m_Axis(vec3(1.f))
	{
	}

	Pipeline::~Pipeline()
	{
	}

	const mat4& Pipeline::getModel()
	{
		// TODO : check if no change => return stored model
		mat4 mat_scale = mat4::scale(m_Scale);
		mat4 mat_rotate = mat4::rotation(m_Angle, m_Axis);
		mat4 mat_translate = mat4::translation(m_WorldPos);

		m_Model = mat_translate * mat_rotate * mat_scale;

		return m_Model;
	}

	const mat4 & Pipeline::getPerspProj()
	{
		m_Proj = mat4::perspective(m_PerspProjData.fov, m_PerspProjData.aspectRatio, m_PerspProjData.near, m_PerspProjData.far);

		return m_Proj;
	}

	const mat4& Pipeline::getView()
	{
		m_View = mat4::lookAt(m_camera.pos, m_camera.target, m_camera.up);

		return m_View;
	}

	void Pipeline::scale(float s)
	{
		scale(s, s, s);
	}

	void Pipeline::scale(float x, float y, float z)
	{
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}

	void Pipeline::scale(const vec3 & vec)
	{
		scale(vec.x, vec.y, vec.z);
	}

	void Pipeline::worldPos(float x, float y, float z)
	{
		m_WorldPos.x = x;
		m_WorldPos.y = y;
		m_WorldPos.z = z;
	}

	void Pipeline::worldPos(const vec3 & pos)
	{
		worldPos(pos.x, pos.y, pos.z);
	}

	void Pipeline::rotate(float angle, float x, float y, float z)
	{
		m_Angle = angle;
		m_Axis.x = x;
		m_Axis.y = y;
		m_Axis.z = z;
	}

	void Pipeline::rotate(float angle, const vec3 & vec)
	{
		rotate(angle, vec.x, vec.y, vec.z);
	}

	void Pipeline::setPerspProj(float fov, float aspectRatio, float near, float far)
	{
		m_PerspProjData.fov = fov;
		m_PerspProjData.aspectRatio = aspectRatio;
		m_PerspProjData.near = near;
		m_PerspProjData.far = far;
	}

	void Pipeline::setPerspProj(const PerspProjData & pdata)
	{
		m_PerspProjData = pdata;
	}

	void Pipeline::setCamera(const xgraphics::Camera& cam)
	{
		m_camera.pos = cam.getPos();
		m_camera.target = cam.getTarget();
		m_camera.up = cam.getUp();

		m_View = mat4::lookAt(m_camera.pos, m_camera.target, m_camera.up);
	}


} }