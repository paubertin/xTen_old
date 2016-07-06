#pragma once

#include "utils.h"
#include "filestream.h"

namespace xten {

	class StreamFactory
	{
	private:
		std::string m_DirName;
	public:
		StreamFactory(const std::string& directoryName): m_DirName(directoryName) {}
		~StreamFactory() {}

		FileStream* open(const std::string& fileName)
		{
			return XNEW FileStream(m_DirName + "/" + fileName);
		}

		std::string getFilePath(const std::string& fileName)
		{
			return (m_DirName + "/" + fileName);
		}
	};

}