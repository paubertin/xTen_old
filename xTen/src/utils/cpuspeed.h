#pragma once

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

#define SLEEPTIME	0


	//========================================================================
	// define static variables
	//========================================================================
	static int	s_milliseconds;
	static __int64	s_ticks;

	static int	s_milliseconds0;
	static __int64	s_ticks0;

	//========================================================================
	// fabs
	//
	// floating point absolute value function
	//========================================================================
#if 1
#pragma message("Disabled local fabs()implementation to prevent collision w/impl in VS.NET 2k3")
	float inline fabs(float a)
	{
		if (a < 0.0f)
			return -a;
		else
			return a;
	}
#endif
	//========================================================================
	// StartTimingCPU
	//
	// Call this function to start timing the CPU. It takes the CPU tick
	// count and the current time and stores it. Then, while you do other
	// things, and the OS task switches, the counters continue to count, and
	// when you call UpdateCPUTime, the measured speed is accurate.
	//
	//========================================================================
	int StartTimingCPU();

	//========================================================================
	// UpdateCPUTime
	//
	// This function stops timing the CPU by adjusting the timers to account
	// for the amount of elapsed time and the number of CPU cycles taked
	// during the timing period.
	//========================================================================
	void UpdateCPUTime();

	//========================================================================
	// CalcCPUSpeed
	//
	// This function takes the measured values and returns a speed that
	// represents a common possible CPU speed.
	//========================================================================
	int CalcCPUSpeed();

	//========================================================================
	// GetCPUSpeed
	//
	// Gets the CPU speed by timing it for 1 second.
	//========================================================================
	int GetCPUSpeed();

	unsigned long ReadCPUSpeed();
}