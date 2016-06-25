#include "camera.h"
#include "../../app/inputmanager.h"


namespace xten { namespace xgraphics {

	const static float MARGIN = 10.f;

	using namespace xmaths;

	Camera::Camera():
		m_Position(vec3(0.f,0.f,10.f)), m_Up(vec3(0.f,1.0f,0.f)), m_Target(vec3(0.f,0.f,1.f)),
		m_Yaw(-90.f), m_Pitch(0.f), m_Speed(0.005f), m_MouseSensitivity(0.05f), m_WheelSensitivity(1.f),
		m_FirstCam(true), m_Active(false), m_Fov(45.0f)
	{
		m_Front = normalize(m_Target - m_Position);
	}

	Camera::~Camera()
	{
	}

	bool Camera::onKeyboard()
	{
		bool res = false;
		if (Input::isKeyPressed(GLFW_KEY_W))
		{
			m_Position += m_Speed * m_Front;
			res = true;
		}
		if (Input::isKeyPressed(GLFW_KEY_S))
		{
			m_Position -= m_Speed * m_Front;
			res = true;
		}
		if (Input::isKeyPressed(GLFW_KEY_A))
		{
			m_Position -= m_Speed * normalize(cross(m_Front, m_Up));
			res = true;
		}
		if (Input::isKeyPressed(GLFW_KEY_D))
		{
			m_Position += m_Speed * normalize(cross(m_Front, m_Up));
			res = true;
		}
		if (Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Position.y -= 0.5*m_Speed;
			res = true;
		}
		if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			m_Position.y += 0.5*m_Speed;
			res = true;
		}

		return res;
	}

	bool Camera::onMouse()
	{
		GLfloat x_offset = 0.f;
		GLfloat y_offset = 0.f;

		if (!(m_FirstCam))
		{
			x_offset = Input::getMousePosition().x - Input::getMouseLastPosition().x;
			y_offset = Input::getMouseLastPosition().y - Input::getMousePosition().y;
		}
		
		Input::getInputManager()->resetMousePosition();
		m_FirstCam = false;

		if (x_offset != 0.f || y_offset != 0.f)
		{
			updateAngles(x_offset, y_offset);
			return true;
		}
		return false;
	}

	void Camera::updateFov(const float dy)
	{
		m_Fov -= m_WheelSensitivity*dy;
		if (m_Fov <= 1.0f)
			m_Fov = 1.0f;
		if (m_Fov >= 70.0f)
			m_Fov = 70.0f;
	}

	void Camera::updateAngles(const float dx, const float dy)
	{
		m_Yaw += dx*m_MouseSensitivity;
		m_Pitch += dy*m_MouseSensitivity;

		if (m_Pitch > 89.5f)
			m_Pitch = 89.5f;
		if (m_Pitch < -89.5f)
			m_Pitch = -89.5f;
	}

	void Camera::onUpdate()
	{
		if (Input::getInputManager()->getMouseWheelYoffset() != 0)
		{
			updateFov((GLfloat)(Input::getInputManager()->getMouseWheelYoffset()));
			Input::getInputManager()->resetWheelY();
			//std::cout << "fov : " << m_Fov << std::endl;
		}

		bool res = false;
		res += onKeyboard(); 
		if (m_Active)
		{
			res += onMouse();
		}
		if (res)
		{
			vec3 front(cosf(deg2rad(m_Yaw))*cosf(deg2rad(m_Pitch)), sinf(deg2rad(m_Pitch)), sin(deg2rad(m_Yaw))*cos(deg2rad(m_Pitch)));
			m_Front = normalize(front);
			m_Target = m_Position + m_Front;
		}
	}

} }