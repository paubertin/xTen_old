#pragma once

#include <vector>
#include <string>

namespace xten {

#define MAX_DIGITS_IN_INT 12
	
	extern std::string ToStr(float num);

	typedef std::vector<std::string> stringVec;

	inline char lowercase(char c);

	bool startsWith(const char* str, const char* prefix, bool ignoreCase = true);

	bool endsWith(const char* str, const char* suffix, bool ignoreCase = true);

	bool endsWith(const std::string& str, const char* suffix, bool ignoreCase = true);

	bool equals(const std::string& a, const char* b);

	bool equals(const std::string& a, const std::string& b);

	bool equalsIgnoreCase(const std::string& a, const char* b);

	std::string getFilenameFromFilePath(const std::string& filepath);

	std::string getFilenameNoExt(const std::string& filename);

}