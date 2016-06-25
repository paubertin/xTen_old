#pragma once

#include "inputmanager.h"
#include "window.h"
#include "../utils/utils.h"

namespace xten {

	enum XTEN_FW_TYPE {
		XTEN_FW_TYPE_GLUT,
		XTEN_FW_TYPE_GLFW
	};

	class Client
	{
	public:
		Client();
		virtual ~Client();
		bool init(XTEN_FW_TYPE type = XTEN_FW_TYPE_GLFW);
		bool createWindow(const std::string& name, const WindowProperties& properties);
		virtual void start();
		void run();
		void close() { m_Running = false; }
		virtual void onUpdate();
		virtual void onRender();
		void shutdown();
		const bool isRunning() const { return m_Running; }
		inline Window* const getWindow() const { return m_Window; }

	private:
		static Client* s_Instance;
		void setCallbacks();
		bool m_Running;
	protected:
		InputManager * m_Input;
		Window * m_Window;
		xgraphics::Camera* m_Camera;
		xmaths::Pipeline* m_Pipeline;
		XTEN_FW_TYPE m_FwType;
	};

}