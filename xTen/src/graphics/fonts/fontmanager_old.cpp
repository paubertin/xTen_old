#include <ft2build.h>
#include FT_FREETYPE_H
#include "fontmanager.h"

namespace xten { namespace xgraphics { namespace xfont {

	// Instantiate static variables
	FontManager* FontManager::s_Instance = nullptr;
	FontAtlas* FontManager::m_Atlas = nullptr;
	FontRenderer* FontManager::m_Renderer = nullptr;
	StreamFactory* FontManager::m_StreamFactory = nullptr;

	FontManager::FontManager(const std::string & dirpath)
	{
		m_StreamFactory = XNEW StreamFactory(dirpath);
		m_Renderer = XNEW FontRenderer();
		m_Atlas = XNEW FontAtlas(m_StreamFactory, 1024, 1024);
	}

	void FontManager::init(const std::string & dirpath)
	{
		clean();
		s_Instance = XNEW FontManager(dirpath);
		m_Renderer->init();
	}

	void FontManager::clean()
	{
		std::cout << "cleaning FontManager" << std::endl;
		XDEL(m_Atlas);
		XDEL(m_Renderer);
		XDEL(s_Instance);
	}

	void FontManager::add(const std::string & name, const std::string & filename, GLuint size)
	{
		m_Atlas->addFont(name, filename, size);
		m_Atlas->create();
	}

	void FontManager::renderText(const std::string & fontname, const std::string & text, GLfloat x, GLfloat y, GLfloat scale, xmaths::vec4 color)
	{
		Font* font = m_Atlas->getFont(fontname);

		FontGeometry* curtext = font->calcVertices(x, y, text, 0x000000, 1.0f);

		m_Renderer->render(curtext);
	}

} } }