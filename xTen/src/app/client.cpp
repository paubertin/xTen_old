#include "client.h"
#include "../system/memorymanager.h"
#include "../utils/cpuspeed.h"
#include "../graphics/API/xten_glfw.h"
#include "../graphics/shaders/shadermanager.h"
#include "../graphics/texturemanager.h"
//#include "../graphics/fonts/fontmanager.h"
//#include "../graphics/fonts/fontrenderer.h"

namespace xten {

	using namespace xgraphics;
	using namespace API;
	//using namespace xfont;

	Client* Client::s_Instance = nullptr;

	Client::Client():
		m_Input(0),  m_Window(0), m_Camera(0), m_Running(false), m_Pipeline(0)
	{
		s_Instance = this;
	}

	Client::~Client()
	{
		ShaderManager::clean();
		TexManager::clean();
		//FontManager::clean();
		XTEN_INFO("Destroying client...");
		shutdown();
	}

	bool Client::init(XTEN_GL_PROFILE gltype, XTEN_GL_FW_TYPE type)
	{
		//std::cout << "Initializing xTen..." << std::endl;
		XTEN_INFO("Initializing xTen...");

		MemoryManager::init();
		MemoryManager::get()->getMemInfo().print();

		unsigned long cpuspeed = ReadCPUSpeed();
		std::cout << "CPU speed : " << cpuspeed << " MHz" << std::endl;

		m_GLProfile = gltype;
		m_FwType = type;
		bool res = false;

		switch (type)
		{
		case XTEN_FW_TYPE_GLFW:
			res = GLFWInit();
			break;
		default:
			res = false;
		}

		return res;
	}

	bool Client::createWindow(const std::string & name, const WindowProperties & properties)
	{
		m_Window = XNEW Window(name, properties);

		bool res = false;

		switch (m_FwType)
		{
		case XTEN_FW_TYPE_GLFW:
			res = GLFWCreateWindow(m_Window, m_GLProfile);
			break;
		default:
			res = false;
		}

		return res;
	}

	bool Client::start()
	{
		if (!m_ATB.init(m_GLProfile))
		{
			XTEN_ERROR("[AntTweakBar]. Failed to initialize ATB.");
			return false;
		}

		m_Input = XNEW InputManager();
		setCallbacks();

		m_Pipeline = XNEW xmaths::Pipeline();
		m_Camera = XNEW Camera();
		m_Running = true;

		GLCall( glClearColor(0.05f, 0.05f, 0.05f, 1.0f) );
		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);
		GLCall( glEnable(GL_CULL_FACE) );
		GLCall( glEnable(GL_DEPTH_TEST) );

		//FontManager::init("fonts");

		return true;
	}

	void Client::setCallbacks()
	{
		XTEN_INFO("Setting callbacks...");
		switch (m_FwType)
		{
		case XTEN_FW_TYPE_GLFW:
			GLFWCallbacks(m_Window);
			break;
		default:
			assert(0);
		}
	}

	void Client::run()
	{
		switch (m_FwType) {
		case XTEN_FW_TYPE_GLFW:
			GLFWRun(this);
			break;
		default:
			assert(0);
		}
	}

	void Client::onUpdate()
	{
		m_Input->onUpdate();
		m_Camera->onUpdate();
	}

	void Client::onRender()
	{
		m_Window->clear();
	}

	void Client::shutdown()
	{
		switch (m_FwType)
		{
		case XTEN_FW_TYPE_GLFW:
			GLFWTerminate(m_Window);
			break;
		default:
			assert(0);
		}

		XDEL(m_Pipeline);
		XDEL(m_Input);
		XDEL(m_Camera);
		XDEL(m_Window);
		MemoryManager::shutdown();
	}

}