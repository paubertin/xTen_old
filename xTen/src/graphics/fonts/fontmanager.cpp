#include "fontmanager.h"

namespace xten { namespace xgraphics { namespace xfont {

	// Instantiate static variables
	FontManager* FontManager::s_Instance = nullptr;
	std::vector<Font*> FontManager::m_Fonts;
	StreamFactory* FontManager::m_StreamFactory = nullptr;

	FontManager::FontManager(const std::string & dirpath)
	{
		m_StreamFactory = XNEW StreamFactory(dirpath);
	}

	void FontManager::init(const std::string & dirpath)
	{
		clean();
		s_Instance = XNEW FontManager(dirpath);
	}

	void FontManager::clean()
	{
		XDEL_VEC(m_Fonts);
		XDEL(m_StreamFactory);
		XDEL(s_Instance);
	}

	void FontManager::add(Font * font)
	{
		m_Fonts.push_back(font);
	}

	void FontManager::add(const std::string & name, const std::string & filename, GLuint size)
	{
		m_Fonts.push_back(XNEW Font(name, m_StreamFactory->getFilePath(filename), size));
	}


} } }