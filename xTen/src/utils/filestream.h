#pragma once

#include "utils.h"

namespace xten {

	class FileStream
	{
	private:
		std::ifstream m_File;
		unsigned int m_FileSize;
	public:
		FileStream(const std::string& path);
		~FileStream();

		const unsigned int size() const;

		int read(void* target, const std::size_t bytes);
	};

}