#include "mu_mutex.h"
#include <windows.h>


mu_mutex::mu_mutex()
{
	g_CS = reinterpret_cast<void*>(new CRITICAL_SECTION);
	CRITICAL_SECTION* pcs = reinterpret_cast<CRITICAL_SECTION*>(g_CS);
	InitializeCriticalSection(pcs);
}


mu_mutex::~mu_mutex()
{
	CRITICAL_SECTION* pcs = reinterpret_cast<CRITICAL_SECTION*>(g_CS);
	delete pcs;
}

void mu_mutex::lock()
{
	CRITICAL_SECTION* pcs = reinterpret_cast<CRITICAL_SECTION*>(g_CS);
	EnterCriticalSection(pcs);
}

void mu_mutex::unlock()
{
	CRITICAL_SECTION* pcs = reinterpret_cast<CRITICAL_SECTION*>(g_CS);
	LeaveCriticalSection(pcs);
}


