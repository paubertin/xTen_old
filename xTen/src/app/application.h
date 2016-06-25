#pragma once

#include <string>

#include <GL\glew.h>

#include "../graphics/API/context.h"
#include "window.h"
#include "inputmanager.h"
#include "../graphics/camera/camera.h"

namespace xten {

	class Application
	{
	private:
		static Application* s_Instance;
	public:
		Application();
		~Application();
		void init();
		inline static Application& getApplication() { return *s_Instance; }

		xgraphics::Camera* m_Camera;
		void onUpdate(Window * window);

	private:
	};

}