#ifdef _WIN32 || _WIN64
#include <direct.h>
#include <Windows.h>
#undef near
#undef far
#endif // _WIN32 || _WIN64

#include <iostream>
#include "memorymanager.h"
#include "../utils/string.h"

namespace xten {

	MemoryManager* MemoryManager::s_Instance = nullptr;

	void MemInfo::print()
	{
		std::cout << "System Memory Info:" << std::endl;
		std::cout << "  Physical memory : " << fromByte(availablePhysicalMemory) 
			<< " / " << fromByte(totalPhysicalMemory) << std::endl;
		std::cout << "  Virtual memory : " << fromByte(availableVirtualMemory)
			<< " / " << fromByte(totalVirtualMemory) << std::endl;
	}

	std::string MemInfo::fromByte(const long long int & value)
	{
		static const float gb = 1024 * 1024 * 1024;
		static const float mb = 1024 * 1024;
		static const float kb = 1024;

		if (value > gb)
		{
			return (ToStr(float(value) / gb) + " GB");
		}
		else if (value > mb)
		{
			return (ToStr(float(value) / mb) + " MB");
		}
		else if (value > kb)
		{
			return (ToStr(float(value) / kb) + " kB");
		}
		else
		{
			return (ToStr(float(value)) + " bytes");
		}
	}

	MemInfo MemoryManager::getMemInfo()
	{
#ifdef _WIN32 || _WIN64
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(status);
		GlobalMemoryStatusEx(&status);

		m_Info.totalPhysicalMemory = (long long int)(status.ullTotalPhys);
		m_Info.availablePhysicalMemory = (long long int)(status.ullAvailPhys);
		m_Info.totalVirtualMemory = (long long int)(status.ullTotalVirtual);
		m_Info.availableVirtualMemory = (long long int)(status.ullAvailVirtual);
#endif // _WIN32 || _WIN64

		return m_Info;
	}

	void MemoryManager::init()
	{
		shutdown();
		s_Instance = XNEW MemoryManager();
	}
	void MemoryManager::shutdown()
	{
		XDEL(s_Instance);
	}
}