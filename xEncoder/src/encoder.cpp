#include "encoder.h"

#ifdef WIN32
#define PATH_MAX    _MAX_PATH
#define realpath(A,B)    _fullpath(B,A,PATH_MAX)
#endif

namespace xten {

	static Encoder* s_Instance;

	Encoder::Encoder(size_t argc, const char** argv):
		m_ParseError(false), m_FontPreview(false)
	{
		s_Instance = this;

		if (argc > 1)
		{
			stringVec arguments;
			for (size_t i = 0; i < argc; ++i)
			{
				arguments.push_back(argv[i]);
			}

			size_t index = 0;
			for (size_t i = 0; i < arguments.size(); ++i)
			{
				if (arguments[i][0] == '-')
				{
					readOption(arguments, &i);
					index = i + 1;
				}
			}

			if (arguments.size() - index == 2)
			{
				setInputFilePath(arguments[index]);
				setOutputFilePath(arguments[index+1]);
			}
			else if (arguments.size() - index == 1)
			{
				setInputFilePath(arguments[index]);
			}
		}
		else
		{
			m_ParseError = true;
		}
	}

	void Encoder::readOption(const stringVec& options, size_t * index)
	{
		const std::string& str = options[*index];
		
		if (str.length() == 0 && str[0] != '-')
		{
			return;
		}

		switch (str[1])
		{
		case 'f':
			if (str.compare("-f:b") == 0)
			{
				m_FontFormat = Font::BITMAP;
			}
			else if (str.compare("-f:d") == 0)
			{
				m_FontFormat = Font::DISTANCE_FIELD;
			}
			break;
		case 'p':
			m_FontPreview = true;
			break;
		case 's':
		{
			const char* sizes = NULL;
			if (str.length() > 2)
			{
				char n = str[2];
				if (n > '0' && n < '9')
				{
					sizes = str.c_str() + 2;
				}
			}
			else
			{
				(*index)++;
				if (*index < options.size())
				{
					sizes = options[*index].c_str();
				}
			}

			if (sizes == NULL)
			{
				std::cerr << "Error: invalid format for arguments: -s" << std::endl;
				m_ParseError = true;
				return;
			}

			// Parse comma-separated list of font sizes
			char* ptr = const_cast<char*>(sizes);
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
					if (size <= 0)
					{
						std::cerr << "Error: invalid font size provided: " << sizeStr.c_str() << std::endl;
						m_ParseError = true;
						return;
					}
					m_FontSizes.push_back((unsigned int)size);
				}
			}
			break;
		}
		default:
			break;
		}

	}
	void Encoder::setInputFilePath(const std::string & inputPath)
	{
		m_FilePath.assign(getRealPath(inputPath));
	}

	std::string Encoder::getOutputFileExtension() const
	{
		switch (getFileFormat())
		{
		case FILEFORMAT_OTF:
		case FILEFORMAT_TTF:
			return ".xtb";
		default:
			return ".xtb";
		}
	}

	bool Encoder::fileExists() const
	{
		if (m_FilePath.length() > 0)
		{
			struct stat buf;
			if (stat(m_FilePath.c_str(), &buf) != -1)
			{
				return true;
			}
		}
		return false;
	}

	void Encoder::setOutputFilePath(const std::string & outputPath)
	{
		std::string ext = getOutputFileExtension();

		if (outputPath.size() > 0 && outputPath[0] != '\0')
		{
			std::string realPath = getRealPath(outputPath);
			if (endsWith(realPath.c_str(), ext.c_str()))
			{
				m_FileOutputPath.assign(realPath);
			}
			else if (endsWith(outputPath.c_str(), "/"))
			{
				std::string fileNameNoExt = getFilenameNoExt(getFilenameFromFilePath(m_FilePath));
				m_FileOutputPath.assign(outputPath);
				m_FileOutputPath.append(fileNameNoExt);
				m_FileOutputPath.append(ext);
			}
			else
			{
				std::string fileNameNoExt = getFilenameNoExt(getFilenameFromFilePath(realPath));
				int pos = realPath.find_last_of("/");
				if (pos)
				{
					m_FileOutputPath = realPath.substr(0, pos);
					m_FileOutputPath.append("/");
					m_FileOutputPath.append(fileNameNoExt);
					m_FileOutputPath.append(ext);
				}
			}
		}
	}

	std::string Encoder::getRealPath(const std::string& filepath)
	{
		char path[PATH_MAX + 1]; /* not sure about the "+ 1" */
		realpath(filepath.c_str(), path);
		replace_char(path, '\\', '/');
		return std::string(path);
	}

	Encoder::FileFormat Encoder::getFileFormat() const
	{
		if (m_FilePath.length() < 5)
		{
			return FILEFORMAT_UNKNOWN;
		}

		std::string ext = "";
		size_t pos = m_FilePath.find_last_of(".");
		if (pos != std::string::npos)
		{
			ext = m_FilePath.substr(pos + 1);
		}
		for (size_t i = 0; i < ext.size(); ++i)
		{
			ext[i] = (char)tolower(ext[i]);
		}

		if (ext.compare("ttf") == 0)
		{
			return FILEFORMAT_TTF;
		}
		else if (ext.compare("otf") == 0)
		{
			return FILEFORMAT_OTF;
		}
		else
		{
			return FILEFORMAT_UNKNOWN;
		}
	}

	void Encoder::replace_char(char* str, char oldChar, char newChar)
	{
		for (; *str != '\0'; ++str)
		{
			if (*str == oldChar)
			{
				*str = newChar;
			}
		}
	}

	std::string Encoder::getOutputFilePath() const
	{
		if (m_FileOutputPath.size() > 0)
		{
			return m_FileOutputPath;
		}
		else
		{
			//generate an output file
			int pos = m_FilePath.find_last_of('.');
			std::string outputFilePath(pos > 0 ? m_FilePath.substr(0, pos) : m_FilePath);

			outputFilePath.append(getOutputFileExtension());
			return outputFilePath;
		}
	}

}