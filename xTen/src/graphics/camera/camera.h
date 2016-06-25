#pragma once

#include "../../maths/xvec.h"

namespace xten { namespace xgraphics {

	class Camera
	{
	private:

		bool m_OnUpperEdge;
		bool m_OnLowerEdge;
		bool m_OnLeftEdge;
		bool m_OnRightEdge;
		
		xmaths::vec3 m_Position;
		xmaths::vec3 m_Target;
		xmaths::vec3 m_Up; 
		xmaths::vec3 m_Front;

		float m_Yaw;
		float m_Pitch;
		float m_Fov;

		float m_Speed;
		float m_MouseSensitivity;
		float m_WheelSensitivity;

		bool m_FirstCam;
		bool m_Active;

		bool onKeyboard();
		bool onMouse();
		void updateAngles(const float dx, const float dy);
		void updateFov(const float dy);

	public:
		Camera();
		~Camera();

		void onUpdate();

		const bool isActive() const { return m_Active; }
		void setActive(bool a) { m_Active = a; if (!a) { init(); } }
		void init() { m_FirstCam = true; }

		void setPos(const xmaths::vec3& pos) { m_Position = pos; }
		void setTarget(const xmaths::vec3& target) { m_Target = normalize(target); }
		void setUp(const xmaths::vec3& up) { m_Up = normalize(up) ; }

		const float getFov() const { return m_Fov; }

		inline const xmaths::vec3& getPos() const { return m_Position; }
		inline const xmaths::vec3& getTarget() const { return m_Target; }
		inline const xmaths::vec3& getUp() const { return m_Up; }
		inline const xmaths::vec3& getFront() const { return m_Front; }

	};

} }