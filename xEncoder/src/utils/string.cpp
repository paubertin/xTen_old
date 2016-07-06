#include "string.h"

namespace xten {

	std::string ToStr(float num)
	{
		char str[64];
		memset(str, 0, 64);
		_sprintf_p(str, 64, "%f", num);
		return (std::string(str));
	}

	char lowercase(char c)
	{
		if (c >= 'A' && c <= 'Z')
		{
			c |= 0x20;
		}
		return c;
	}

	bool startsWith(const char* str, const char* prefix, bool ignoreCase)
	{
		size_t length = strlen(str);
		size_t prefixLength = strlen(prefix);

		if (prefixLength > length)
		{
			return false;
		}

		const char* p = str;
		while (*p != '\0' && *prefix != '\0')
		{
			if (ignoreCase)
			{
				if (lowercase(*p) != lowercase(*prefix))
				{
					return false;
				}
			}
			else if (*p != *prefix)
			{
				return false;
			}

			++p;
			++prefix;
		}
		return true;
	}

	bool endsWith(const char* str, const char* suffix, bool ignoreCase)
	{
		size_t length = strlen(str);
		size_t suffixLength = strlen(suffix);

		if (suffixLength > length)
		{
			return false;
		}

		size_t offset = length - suffixLength;

		const char* p = str + offset;
		while (*p != '\0' && *suffix != '\0')
		{
			if (ignoreCase)
			{
				if (lowercase(*p) != lowercase(*suffix))
				{
					return false;
				}
			}
			else if (*p != *suffix)
			{
				return false;
			}

			++p;
			++suffix;
		}
		return true;
	}
	bool endsWith(const std::string& str, const char* suffix, bool ignoreCase)
	{
		return endsWith(str.c_str(), suffix, ignoreCase);
	}

	bool equals(const std::string& a, const char* b)
	{
		return (a.compare(b) == 0);
	}

	bool equals(const std::string& a, const std::string& b)
	{
		return (a.compare(b) == 0);
	}

	bool equalsIgnoreCase(const std::string& a, const char* b)
	{
		size_t bLength = strlen(b);
		if (a.size() != bLength)
		{
			return false;
		}
		for (size_t i = 0; i < bLength; ++i)
		{
			if (lowercase(a[i]) != lowercase(b[i]))
			{
				return false;
			}
		}
		return true;
	}

	std::string getFilenameFromFilePath(const std::string& filepath)
	{
		if (filepath.find_last_of("/") != std::string::npos)
		{
			return filepath.substr(filepath.find_last_of("/") + 1);
		}
		return "";
	}

	std::string getFilenameNoExt(const std::string& filename)
	{
		if (filename.find_last_of(".") != std::string::npos)
		{
			return filename.substr(0, filename.find_last_of("."));
		}
		return filename;
	}

	std::string getBaseName(const std::string& filepath)
	{
		size_t index1 = filepath.find_last_of('\\');
		size_t index2 = filepath.find_last_of('/');
		size_t index = (index1 != -1 && index1 > index2 ? index1 : index2);
		size_t length = filepath.length();
		std::string filename = filepath.substr(index + 1, length);
		length = filename.length();
		std::string output = filename.substr(0, (length - 4));
		return output;
	}

}