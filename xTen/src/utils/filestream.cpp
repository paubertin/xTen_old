#include "filestream.h"

namespace xten {

	FileStream::FileStream(const std::string& path):
		m_File(path.c_str(), std::ios::binary), m_FileSize(0)
	{
		if (m_File)
		{
			m_File.seekg(0, m_File.end);
			m_FileSize = m_File.tellg();
			m_File.seekg(0, m_File.beg);
		}
	}

	FileStream::~FileStream()
	{
		m_File.close();
	}

	const unsigned int FileStream::size() const
	{
		return m_FileSize;
	}

	int FileStream::read(void * target, const std::size_t bytes)
	{
		m_File.read((char*)target, bytes);
		int readBytes = bytes;

		if (m_File.fail())
		{
			readBytes = -1;
		}

		return readBytes;
	}



}