#include "cpuspeed.h"

#ifdef _WIN32 || _WIN64
#include <Windows.h>
#endif // _WIN32 || _WIN64

//========================================================================
// CPUSPEED
//
// CPU Timer for the Action, Arcade, Strategy Games Group, a part of
// the Entertainment Business Unit at Microsoft.
//
// (c) Copyright 1999-2000 Microsoft Corporation.
// Written by Michael Lyons 
//
// USED WITH PERMISSION
//
//========================================================================

//========================================================================
//  Content References in Game Coding Complete 2nd Edition
// 
//  GetCPUSpeed					- Chapter 5, page 135
//========================================================================

namespace xten {

	int StartTimingCPU()
	{
#ifdef _WIN32 || _WIN64
		//
		// detect ability to get info
		//
		__asm
		{
			pushfd; push extended flags
			pop		eax; store eflags into eax
			mov		ebx, eax; save EBX for testing later
			xor		eax, (1 << 21); switch bit 21
			push	eax; push eflags
			popfd; pop them again
			pushfd; push extended flags
			pop		eax; store eflags into eax
			cmp		eax, ebx; see if bit 21 has changed
			jz		no_cpuid; make sure it's now on
		}
		//
		// make ourselves high priority just for the time between
		// when we measure the time and the CPU ticks
		//
		DWORD dwPriorityClass = GetPriorityClass(GetCurrentProcess());
		int dwThreadPriority = GetThreadPriority(GetCurrentThread());
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		//
		// start timing
		//
		s_milliseconds0 = (int)timeGetTime();
		__asm
		{
			lea		ecx, s_ticks0; get the offset
			mov		dword ptr[ecx], 0; zero the memory
			mov		dword ptr[ecx + 4], 0;
			rdtsc; read time - stamp counter
				mov[ecx], eax; store the negative
				mov[ecx + 4], edx; in the variable
		}
		//
		// restore thread priority
		//
		SetThreadPriority(GetCurrentThread(), dwThreadPriority);
		SetPriorityClass(GetCurrentProcess(), dwPriorityClass);
		return 0;
	no_cpuid:
		return -1;
#endif
	}

	void UpdateCPUTime()
	{
#ifdef _WIN32 || _WIN64
		//
		// make ourselves high priority just for the time between
		// when we measure the time and the CPU ticks
		//
		DWORD dwPriorityClass = GetPriorityClass(GetCurrentProcess());
		int dwThreadPriority = GetThreadPriority(GetCurrentThread());
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		//
		// get the times
		//
		s_milliseconds = -s_milliseconds0;
		s_ticks = -s_ticks0;
		s_milliseconds += (int)timeGetTime();
		__asm
		{
			lea		ecx, s_ticks; get the offset
			rdtsc; read time - stamp counter
			add[ecx], eax; add the tick count
			adc[ecx + 4], edx;
		}
		//
		// restore thread priority
		//
		SetThreadPriority(GetCurrentThread(), dwThreadPriority);
		SetPriorityClass(GetCurrentProcess(), dwPriorityClass);
		return;
#endif
	}

	int CalcCPUSpeed()
	{
#ifdef _WIN32 || _WIN64
		//
		// get the actual cpu speed in MHz, and
		// then find the one in the CPU speed list
		// that is closest
		//
		const struct tagCPUSPEEDS
		{
			float	fSpeed;
			int		iSpeed;
		} cpu_speeds[] =
		{
			//
			// valid CPU speeds that are not integrally divisible by
			// 16.67 MHz
			//
			{ 60.00f,	 60 },
			{ 75.00f,	 75 },
			{ 90.00f,	 90 },
			{ 120.00f,	120 },
			{ 180.00f,	180 },
		};
		//
		// find the closest one
		//
		float	fSpeed = ((float)s_ticks) / ((float)s_milliseconds*1000.0f);
		int	iSpeed = cpu_speeds[0].iSpeed;
		float	fDiff = (float)fabs(fSpeed - cpu_speeds[0].fSpeed);
		for (int i = 1; i < sizeof(cpu_speeds) / sizeof(cpu_speeds[0]); i++)
		{
			float fTmpDiff = (float)fabs(fSpeed - cpu_speeds[i].fSpeed);
			if (fTmpDiff < fDiff)
			{
				iSpeed = cpu_speeds[i].iSpeed;
				fDiff = fTmpDiff;
			}
		}
		//
		// now, calculate the nearest multiple of fIncr
		// speed
		//
		//
		// now, if the closest one is not within one incr, calculate
		// the nearest multiple of fIncr speed and see if that's
		// closer
		//
		const float fIncr = 16.66666666666666666666667f;
		const int iIncr = 4267; // fIncr << 8

								//if (fDiff > fIncr)
		{
			//
			// get the number of fIncr quantums the speed is
			//
			int	iQuantums = (int)((fSpeed / fIncr) + 0.5f);
			float	fQuantumSpeed = (float)iQuantums * fIncr;
			float	fTmpDiff = (float)fabs(fQuantumSpeed - fSpeed);

			if (fTmpDiff < fDiff)
			{
				iSpeed = (iQuantums * iIncr) >> 8;
				fDiff = fTmpDiff;
			}
		}

		return iSpeed;
#endif
	}

	int GetCPUSpeed()
	{
#ifdef _WIN32 || _WIN64
		static int CPU_SPEED = 0;

		if (CPU_SPEED != 0)
		{
			//This will assure that the 0.5 second delay happens only once
			return CPU_SPEED;
		}

		if (StartTimingCPU())
			return 0;

		//This will lock the application for 1 second
		do
		{
			UpdateCPUTime();
			Sleep(SLEEPTIME);
		} while (s_milliseconds < 1000);

		CPU_SPEED = CalcCPUSpeed();
		return CPU_SPEED;
#endif
	}

#ifdef _WIN32 || _WIN64
	unsigned long ReadCPUSpeed()
	{
		unsigned long BufSize = sizeof(unsigned long);
		unsigned long dwMHz = 0;
		unsigned long type = REG_DWORD;
		HKEY hKey;
		// open the key where the proc speed is hidden:
		long lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
			0, KEY_READ, &hKey);
		if (lError == ERROR_SUCCESS)
		{
			// query the key:
			RegQueryValueEx(hKey, "~MHz", NULL, &type, (LPBYTE)&dwMHz, &BufSize);
		}
		return dwMHz;
	}
#endif

}