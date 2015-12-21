#include "Mutex.h"

#ifdef WIN32

#include "windows.h"

//-----------------------------------------------------------------------------------------------
// Critical section constructor
Mutex::Mutex() : m_data(NULL)
{
	m_data= new CRITICAL_SECTION;
	::InitializeCriticalSection((CRITICAL_SECTION*)m_data);
}

//-----------------------------------------------------------------------------------------------
// Critical section deconstructor
Mutex::~Mutex()
{
	::DeleteCriticalSection((CRITICAL_SECTION*)m_data);
	delete m_data;
}

//-----------------------------------------------------------------------------------------------
//	Critical section enter
void Mutex::enter()
{
	::EnterCriticalSection((CRITICAL_SECTION*)m_data);
}

//-----------------------------------------------------------------------------------------------
//	Critical section leave
void Mutex::leave()
{
	::LeaveCriticalSection((CRITICAL_SECTION*)m_data);
}

#endif