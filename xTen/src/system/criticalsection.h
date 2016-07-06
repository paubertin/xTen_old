#pragma once

#include "../xTenStd.h"

namespace xten {

	class CriticalSection: public xNonCopyable
	{
	public:
		CriticalSection()
		{
			InitializeCriticalSection(&m_Cs);
		}

		~CriticalSection()
		{
			DeleteCriticalSection(&m_Cs);
		}

		void lock()
		{
			EnterCriticalSection(&m_Cs);
		}

		void unlock()
		{
			LeaveCriticalSection(&m_Cs);
		}

	protected:
		mutable CRITICAL_SECTION m_Cs;
	};

	/*
	Description

	helper class


	allows automatic Locking/ Unlocking of a Resource,
	protected by a Critical Section:
	- locking when this object gets constructed
	- unlocking when this object is destructed
	(goes out of scope)


	Usage


	when you need protected access to a resource, do the following
	1. Create a Critical Section associated with the resource
	2. Embody the code accessing the resource in braces {}
	3. Construct an ScopedCriticalSection object


	Example:
	// we assume that m_CriticalSection
	// is a private variable, and we use it to protect
	// 'this' from being accessed while we need safe access to a resource



	// code that does not need resource locking

	{
	ScopedCriticalSection I_am_locked( m_cs);

	// code that needs thread locking
	}

	// code that does not need resource locking


	*/

	class ScopedCriticalSection : public xNonCopyable
	{
	public:
		ScopedCriticalSection(CriticalSection & csResource):
			m_CsResource(csResource)
		{
			m_CsResource.lock();
		}
		~ScopedCriticalSection()
		{
			m_CsResource.unlock();
		}
	private:
		CriticalSection & m_CsResource;
	};

}