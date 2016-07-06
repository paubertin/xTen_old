#pragma once

#include "utils\utils.h"
#include "objects\font.h"

namespace xten {

	class Encoder
	{
	public:
		enum FileFormat
		{
			FILEFORMAT_UNKNOWN,
			FILEFORMAT_TTF,
			FILEFORMAT_OTF
		};
	public:
		static std::string getRealPath(const std::string& filepath);

		FileFormat getFileFormat() const;
		std::string getOutputFileExtension() const;

		inline bool parseError() const { return m_ParseError; }
		
		bool fileExists() const;
		inline bool fontPreviewEnabled() const { return m_FontPreview; }

		inline const char* getFilePathPointer() const { return m_FilePath.c_str(); }
		inline const std::string& getFilePath() const { return m_FilePath; }
		inline std::vector<unsigned int> getFontSizes() const {	return m_FontSizes; }
		inline Font::FontFormat getFontFormat() const { return m_FontFormat; }

		std::string getOutputFilePath() const;

	private:
		/**
		* Reads the command line option from the list of options starting at the given index.
		*
		* @param options The list of command line options.
		* @param index Pointer to the index within the options list. The index will be changed
		*              if an option takes multiple arguments.
		*/
		void readOption(const stringVec& options, size_t *index);
		void setInputFilePath(const std::string& inputPath);
		void setOutputFilePath(const std::string& outputPath);
		static void replace_char(char* str, char oldChar, char newChar);
	private:
		bool m_ParseError;
		bool m_FontPreview;
		std::vector<unsigned int> m_FontSizes;
		Font::FontFormat m_FontFormat;
		std::string m_FilePath;
		std::string m_FileOutputPath;
	public:
		Encoder(size_t argc, const char** argv);
	};

}