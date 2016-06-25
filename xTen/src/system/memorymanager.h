#pragma once

#include "../utils/utils.h"

namespace xten {

	struct MemInfo
	{
		long long int totalPhysicalMemory;
		long long int availablePhysicalMemory;

		long long int totalVirtualMemory;
		long long int availableVirtualMemory;

		void print();
		std::string fromByte(const long long int & value);
	};

	class MemoryManager
	{
	public:
		static MemoryManager* s_Instance;

		MemoryManager(): m_Info() {}

		MemInfo getMemInfo();

		static void init();
		static void shutdown();

		static MemoryManager* get() { return s_Instance; }
	private:
		MemInfo m_Info;

	};

}