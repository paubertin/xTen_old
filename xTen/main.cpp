#include "src\xten.h"

using namespace xten;
using namespace xgraphics;
using namespace xmaths;

class Game : public Client
{
private:
	LightingTechnique * m_Lightning;
	DirectionalLight m_DirLight;
	float m_scale, depth;
	Mesh * m_Mesh;
	PointLight pl[2];
	SpotLight sl;
public:
	Game() :
		Client(), m_scale(0.0f), m_Lightning(nullptr), m_DirLight(), m_Mesh(nullptr), depth(10.f)
	{}

	~Game()
	{
		XDEL(m_Lightning);
		XDEL(m_Mesh);
		std::cout << "destroying game" << std::endl;
	}

	void start() override
	{
		Client::start();

		m_DirLight.m_AmbientIntensity = 0.05f;
		m_DirLight.m_DiffuseIntensity = 0.85f;
		m_DirLight.m_Color = vec3(g_White);
		m_DirLight.m_Direction = vec3(1.0f, -1.0f, 0.f);

		pl[0].m_AmbientIntensity = 0.05f;
		pl[0].m_DiffuseIntensity = 0.25f;
		pl[0].m_Color = vec3(1.0f, 0.5f, 0.0f);
		pl[0].m_Attenuation.Const = 1.0f;
		pl[0].m_Attenuation.Lin = 0.045f;
		pl[0].m_Attenuation.Quad = 0.0075f;
		
		pl[1].m_AmbientIntensity = 0.05f;
		pl[1].m_DiffuseIntensity = 0.25f;
		pl[1].m_Color = vec3(0.0f, 0.5f, 1.0f);
		pl[1].m_Attenuation.Const = 1.0f;
		pl[1].m_Attenuation.Lin = 0.045f;
		pl[1].m_Attenuation.Quad = 0.0075f;

		sl.m_AmbientIntensity = 0.05f;
		sl.m_DiffuseIntensity = 1.0f;
		sl.m_Color = vec3(g_Red);
		sl.m_Attenuation.Const = 1.0f;
		sl.m_Attenuation.Lin = 0.045f;
		sl.m_Attenuation.Quad = 0.0075f;
		sl.m_Cutoff = 10.0f;

		m_Lightning = XNEW LightingTechnique();
		m_Lightning->init();
		m_Lightning->use();
		m_Lightning->setTextureUnit(0);
		m_Lightning->setDirLight(m_DirLight);

		m_Mesh = XNEW Mesh();
		m_Mesh->loadMesh("models/phoenix_ugv.md2");
		//m_Mesh->loadMesh("models/nanosuit/nanosuit.obj");

	}

	void onUpdate() override
	{
		Client::onUpdate();

		if (Input::isMouseButtonClicked(GLFW_MOUSE_BUTTON_1))
		{
			std::cout << "mouse typed !" << std::endl;
			if (m_Camera->isActive())
			{
				m_Camera->setActive(false);
			}
			else
			{
				m_Camera->setActive(true);
			}
		}

	}

	void onRender() override
	{
		m_scale += 0.001f;

		Client::onRender();

		pl[0].m_Position = vec3(3.0f, 1.0f, - depth * (cosf(m_scale) + 1.0f) / 2.0f);
		pl[1].m_Position = vec3(7.0f, 1.0f, - depth * (sinf(m_scale) + 1.0f) / 2.0f);

		sl.m_Position = m_Camera->getPos();
		sl.m_Direction = m_Camera->getFront();

		m_Pipeline->scale(0.1f);
		m_Pipeline->rotate(m_scale, g_Y);
		m_Pipeline->worldPos(0.0f, 0.0f, -10.0f);
		m_Pipeline->setCamera(*m_Camera);
		m_Pipeline->setPerspProj(m_Camera->getFov(), m_Window->getRatio(), 0.1f, 500.0f);

		m_Lightning->use();
		m_Lightning->setSpotLights(1, &sl);
		m_Lightning->setPointLights(2, pl);
		m_Lightning->setPVM(m_Pipeline->getPerspProj(), m_Pipeline->getView(), m_Pipeline->getModel());
		m_Lightning->setViewPos(m_Camera->getPos());
		m_Lightning->setModelMat(m_Pipeline->getModel());
		m_Lightning->setMatSpecularIntensity(1.0f);
		m_Lightning->setMatSpecularPower(32);

		m_Mesh->render();

	}

};

int main(int argc, char** argv)
{
	Client * game = XNEW Game;

	if (!game->init(XTEN_FW_TYPE_GLFW))
	{
		std::cout << "Failed to init application" << std::endl;
		system("PAUSE");
		return 1;
	}

	if (!game->createWindow("xTen Game !", {1200,675,false,false}))
	{
		std::cout << "Failed to create window" << std::endl;
		system("PAUSE");
		return 1;
	}

	game->start();
	game->run();

	XDEL(game);

	system("PAUSE");

	return 0;
}
