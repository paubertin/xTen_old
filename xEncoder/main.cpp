#include "src\xencoder.h"

#define FONT_SIZE_DISTANCEFIELD 48

static std::vector<unsigned int> promptUserFontSize();

using namespace xten;

int main(int argc, const char** argv)
{
	Encoder encoder(argc, argv);

	if (encoder.parseError())
	{
		std::cerr << "Parse Error. Encoder will close." << std::endl;
		system("PAUSE");
		return 0;
	}

	if (!encoder.fileExists())
	{
		std::cerr << "Error: file "<< encoder.getFilePathPointer() << "not found. Encoder will close. " << std::endl;
		system("PAUSE");
		return -1;
	}

	std::cout << "Encoding file " << encoder.getFilePathPointer() << std::endl;

	switch (encoder.getFileFormat())
	{
	case Encoder::FILEFORMAT_OTF:
	case Encoder::FILEFORMAT_TTF:
	{
		std::vector<unsigned int> fontSizes = encoder.getFontSizes();

		Font::FontFormat fontFormat = encoder.getFontFormat();
		if (fontFormat == Font::BITMAP)
		{
			if (fontSizes.size() == 0)
			{
				fontSizes = promptUserFontSize();
			}
		}
		else
		{
			if (fontSizes.size() == 0)
			{
				fontSizes.push_back(FONT_SIZE_DISTANCEFIELD);
			}
		}
		std::string id = getBaseName(encoder.getFilePath());
		// write font
		writeFont(encoder.getFilePath().c_str(), encoder.getOutputFilePath().c_str(), fontSizes, id.c_str(), encoder.fontPreviewEnabled(), fontFormat);
		break;
	}
	default:
	{
		std::cerr << "Error: Unsupported file format. " << std::endl;
		system("PAUSE");
		return -1;
	}
	}


	return 0;
}

/**
* Prompts the user for a font size until a valid font size is entered.
*
* @return A valid font size.
*/
std::vector<unsigned int> promptUserFontSize()
{
	static const int lowerBound = 8;
	static const int upperBound = 96;
	std::vector<unsigned int> fontSizes;
	bool parseError = false;
	char buffer[4096];
	do
	{
		parseError = false;
		fontSizes.clear();

		// Prompt for font sizes
		printf("Enter a comma-separated list of font sizes (pixels) to generate:\n");
		std::cin.getline(buffer, 4096);

		// Parse comma-separated list of fonts sizes and validate
		char* ptr = const_cast<char*>(buffer);
		std::string sizeStr;
		while (ptr)
		{
			char* end = strchr(ptr, ',');
			if (end)
			{
				sizeStr = std::string(ptr, end - ptr);
				ptr = end + 1;
			}
			else
			{
				sizeStr = ptr;
				ptr = NULL;
			}
			if (sizeStr.length() > 0)
			{
				int size = atoi(sizeStr.c_str());
				if (size < lowerBound || size > upperBound)
				{
					printf("Invalid font size: %d. Must be between %d-%d.\n", size, lowerBound, upperBound);
					parseError = true;
					continue;
				}
				fontSizes.push_back((unsigned int)size);
			}
		}
	} while (parseError);

	return fontSizes;
}