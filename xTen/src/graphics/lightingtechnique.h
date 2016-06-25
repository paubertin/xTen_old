#pragma once

#include "lights.h"
#include "shaders/shader.h"


namespace xten { namespace xgraphics {

	class LightingTechnique
	{
	private:
		Shader *m_Shader;
	public:
		LightingTechnique():m_Shader(nullptr) {}
		~LightingTechnique() {}

		void init();
		void setPVM(const xmaths::mat4 & persp, const xmaths::mat4 & view, const xmaths::mat4 & model);
		void setModelMat(const xmaths::mat4 & model);
		void setViewPos(const xmaths::vec3 & viewpos);

		void setDirLight(const DirectionalLight & dirlight);
		void setPointLights(GLuint numLights, const PointLight* plights);
		void setSpotLights(GLuint numLights, const SpotLight* slights);

		void setMatSpecularIntensity(GLfloat intensity);
		void setMatSpecularPower(GLuint power);
		void setTextureUnit(GLuint textureunit);

		void use() { m_Shader->enable(); }
	};

} }