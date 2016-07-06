#include "font.h"

namespace xten { namespace xgraphics { namespace xfont {

	Font::Font(std::string name, std::string filename, float size) :
		m_Name(name), m_FileName(filename), m_Size(size), m_Scale(xmaths::vec2(1, 1))
	{
		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 2);
		m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, size, filename.c_str()); // "./fonts/cnr.otf"
	}

	void Font::setScale(float x, float y)
	{
		m_Scale = xmaths::vec2(x, y);
	}

} } }