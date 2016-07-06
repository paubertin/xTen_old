#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "objects\font.h"

#define START_INDEX     32
#define END_INDEX       127
#define GLYPH_PADDING   4

namespace xten {

	struct TTFGlyph
	{
		unsigned int index;
		unsigned int width;
		int bearingX;
		unsigned int advance;
		float uvCoords[4];
	};

	struct FontData
	{
		// Array of glyphs for a font
		TTFGlyph glyphArray[END_INDEX - START_INDEX];

		// Stores final height of a row required to render all glyphs
		int fontSize;

		// Actual size of the underlying glyphs (may be different from fontSize)
		int glyphSize;

		// Font texture
		unsigned char* imageBuffer;
		unsigned int imageWidth;
		unsigned int imageHeight;

		FontData() : fontSize(0), glyphSize(0), imageBuffer(NULL), imageWidth(0), imageHeight(0)
		{
		}

		~FontData()
		{
			if (imageBuffer)
				free(imageBuffer);
		}
	};

	/**
	* Writes the xfont file.
	*
	* @param inFilePath Input file path to the tiff file.
	* @param outFilePath Output file path to write the gpb to.
	* @param fontSizes List of sizes to generate for the font.
	* @param id ID string of the font in the ref table.
	* @param fontpreview True if the pgm font preview file should be written. (For debugging)
	*
	* @return 0 if successful, -1 if error.
	*/
	int writeFont(const char* inFilePath, const char* outFilePath, std::vector<unsigned int>& fontSize, const char* id, bool fontpreview, Font::FontFormat fontFormat);

}