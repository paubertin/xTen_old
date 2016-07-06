#pragma once

#include "../../../ext/freetype-gl/freetype-gl.h"
#include "../../utils/utils.h"

namespace xten { namespace xgraphics { namespace xfont {

	class Font
	{
	private:
		ftgl::texture_atlas_t* m_FTAtlas;
		ftgl::texture_font_t* m_FTFont;
		float m_Size;
		xmaths::vec2 m_Scale;
		std::string m_Name;
		std::string m_FileName;
	public:
		Font(std::string name, std::string filename, float size);

		void setScale(float x, float y);

		inline ftgl::texture_font_t* getFTFont() const { return m_FTFont; }
		//inline ftgl::texture_atlas_t* getFTAtlas() const { return m_FTAtlas; }

		inline const unsigned int getID() const { return m_FTAtlas->id; }
		inline const xmaths::vec2& getScale() const { return m_Scale; }
		inline const std::string& getName() { return m_Name; }
		inline const std::string& getFileName() { return m_FileName; }
		inline const float getSize() { return m_Size; }
	};

} } }