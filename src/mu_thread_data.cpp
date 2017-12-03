#include "mu_thread_data.h"
#include <windows.h>
#include "mu_mutex_locker.h"

static DWORD g_TlsIndex = 0;

mu_thread_data::mu_thread_data() :m_ref(0), m_pDispatcher(0)
{
}


mu_thread_data::~mu_thread_data()
{
}

mu_thread_data* mu_thread_data::get_current()
{
	mu_mutex_locker locker(g_pMutex);
	if (!g_TlsIndex)
	{
		g_TlsIndex = TlsAlloc();
	}
	
	mu_thread_data* pThreadData = reinterpret_cast<mu_thread_data*>(TlsGetValue(g_TlsIndex));
	
	if (!pThreadData)
	{
		pThreadData = new mu_thread_data;
		pThreadData->set_thread_id(GetCurrentThreadId());
		TlsSetValue(g_TlsIndex, pThreadData);
	}

	return pThreadData;
}

void mu_thread_data::add_ref()
{
	m_ref++;
}

void mu_thread_data::decrease_ref()
{
	m_ref--;
	if (!m_ref)
	{
		mu_mutex_locker locker(g_pMutex);
		mu_thread_data* pThreadData = reinterpret_cast<mu_thread_data*>(TlsGetValue(g_TlsIndex));
		delete pThreadData;
		TlsSetValue(g_TlsIndex, 0);
	}
}

unsigned int mu_thread_data::get_thread_id()
{
	return m_nThreadId;
}

void mu_thread_data::set_thread_id(int nThreadId)
{
	m_nThreadId = nThreadId;
}

void mu_thread_data::set_event_dispatcher(mu_event_dispatcher* pDispatcher)
{
	m_pDispatcher = pDispatcher;
}

mu_event_dispatcher* mu_thread_data::get_event_dispatcher()
{
	return m_pDispatcher;
}

mu_mutex_ptr mu_thread_data::g_pMutex = mu_mutex_ptr(new mu_mutex);

mu_type* mu_event::get_type()
{
	return m_pType;
}

void mu_event::set_type(mu_type* pType)
{
	m_pType = pType;
}

mu_metaobject* mu_event::get_metaobj()
{
	return m_pMetaObj;
}

void mu_event::set_metaobj(mu_metaobject* pMetaObj)
{
	m_pMetaObj = pMetaObj;
}

int mu_event::get_method_index()
{
	return m_nMethodIndex;
}

void mu_event::set_method_index(int nMethodIndex)
{
	m_nMethodIndex = nMethodIndex;
}

void mu_event::set_receiver(mu_object* pReceiver)
{
	m_pReceiver = pReceiver;
}

mu_object* mu_event::get_receiver()
{
	return m_pReceiver;
}
