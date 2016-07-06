#pragma once

#include "../../utils/utils.h"
#include "font.h"
#include "../../utils/streamfactory.h"
#include "../FontBatchRenderer.h"

namespace xten { namespace xgraphics { namespace xfont {

	class FontManager
	{
	private:
		static std::vector<Font*> m_Fonts;
		static StreamFactory* m_StreamFactory;
		static FontBatchRenderer* m_Renderer;
		static FontManager* s_Instance;
	public:
		static void init(const std::string & dirpath);
		static void add(Font* font);
		static void add(const std::string & name, const std::string & filename, GLuint size);
		//static Font* get();
		//static Font* get(const std::string & name);
		//static Font* get(const std::string & name, unsigned int size);
		static void clean();
	private:
		FontManager(const std::string & dirpath);
		~FontManager() { }
	};

} } }