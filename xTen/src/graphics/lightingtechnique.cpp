#include "lightingtechnique.h"
#include "shaders\shadermanager.h"

namespace xten { namespace xgraphics {

	void LightingTechnique::init()
	{
		ShaderManager::add("light", "shaders/light.vert", "shaders/light.frag");
		m_Shader = ShaderManager::get("light");
	}

	void LightingTechnique::setPVM(const xmaths::mat4 & persp, const xmaths::mat4 & view, const xmaths::mat4 & model)
	{
		xmaths::mat4 pvm = persp * view * model;
		m_Shader->setMat4f("pvm", pvm);
	}

	void LightingTechnique::setModelMat(const xmaths::mat4 & model)
	{
		m_Shader->setMat4f("model", model);
	}

	void LightingTechnique::setViewPos(const xmaths::vec3 & viewpos)
	{
		m_Shader->setVector3f("viewpos", viewpos);
	}

	void LightingTechnique::setDirLight(const DirectionalLight & dirlight)
	{
		m_Shader->setVector3f("dirlight.Base.Color", dirlight.m_Color);
		m_Shader->setFloat("dirlight.Base.AmbientIntensity", dirlight.m_AmbientIntensity);
		m_Shader->setFloat("dirlight.Base.DiffuseIntensity", dirlight.m_DiffuseIntensity);
		m_Shader->setVector3f("dirlight.Direction", normalize(dirlight.m_Direction));
	}

	void LightingTechnique::setPointLights(GLuint numLights, const PointLight * plights)
	{
		m_Shader->setInteger("numPointLights", numLights);

		for (GLuint i = 0; i < numLights; ++i)
		{
			char Name[128];
			memset(Name, 0, sizeof(Name));

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Base.Color", i);
			m_Shader->setVector3f(Name, plights[i].m_Color);

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Base.AmbientIntensity", i);
			m_Shader->setFloat(Name, plights[i].m_AmbientIntensity);

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Base.DiffuseIntensity", i);
			m_Shader->setFloat(Name, plights[i].m_DiffuseIntensity);

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Position", i);
			m_Shader->setVector3f(Name, plights[i].m_Position);

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Att.Const", i);
			m_Shader->setFloat(Name, plights[i].m_Attenuation.Const);

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Att.Lin", i);
			m_Shader->setFloat(Name, plights[i].m_Attenuation.Lin);

			_snprintf_s(Name, sizeof(Name), "pointLights[%d].Att.Quad", i);
			m_Shader->setFloat(Name, plights[i].m_Attenuation.Quad);
		}
	}

	void LightingTechnique::setSpotLights(GLuint numLights, const SpotLight* slights)
	{
		m_Shader->setInteger("numSpotLights", numLights);

		for (GLuint i = 0; i < numLights; ++i)
		{
			char Name[128];
			memset(Name, 0, sizeof(Name));

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Base.Color", i);
			m_Shader->setVector3f(Name, slights[i].m_Color);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Base.AmbientIntensity", i);
			m_Shader->setFloat(Name, slights[i].m_AmbientIntensity);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Base.DiffuseIntensity", i);
			m_Shader->setFloat(Name, slights[i].m_DiffuseIntensity);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Position", i);
			m_Shader->setVector3f(Name, slights[i].m_Position);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Att.Const", i);
			m_Shader->setFloat(Name, slights[i].m_Attenuation.Const);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Att.Lin", i);
			m_Shader->setFloat(Name, slights[i].m_Attenuation.Lin);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Point.Att.Quad", i);
			m_Shader->setFloat(Name, slights[i].m_Attenuation.Quad);

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Direction", i);
			m_Shader->setVector3f(Name, normalize(slights[i].m_Direction));

			_snprintf_s(Name, sizeof(Name), "spotLights[%d].Cutoff", i);
			m_Shader->setFloat(Name, cosf(deg2rad(slights[i].m_Cutoff)));
		}
	}

	void LightingTechnique::setMatSpecularIntensity(GLfloat intensity)
	{
		m_Shader->setFloat("matSpecularIntensity", intensity);
	}

	void LightingTechnique::setMatSpecularPower(GLuint power)
	{
		m_Shader->setInteger("matSpecularPower", power);
	}

	void LightingTechnique::setTextureUnit(GLuint textureunit)
	{
		m_Shader->setInteger("texture", textureunit);
	}

} }