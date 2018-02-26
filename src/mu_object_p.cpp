#include "mu_object_p.h"
#include "mu_mutex.h"
#include "mu_mutex_locker.h"
#include "mu_object.h"
#include "mu_warn.h"
#include "mu_thread_data.h"

mu_object_p::mu_object_p(mu_object* pOwner) :m_pOwner(pOwner),
m_pSendersMutex(new mu_mutex), m_pConnectMethodsMutex(new mu_mutex),
m_bIsInSignal(false), m_bIsDeleteLater(false)
{

}


mu_object_p::~mu_object_p()
{
	if (m_bIsDeleteLater)
	{
		return;
	}
	{
		mu_mutex_locker locker(m_pSendersMutex);
		for (auto it = m_listSenders.begin(); it != m_listSenders.end(); ++it)
		{
			mu_object* pSender = (*it)->get_obj();
			mu_object_p* pSenerP = mu_object_p::get(pSender->get_p());
			pSenerP->disconnect((*it)->get_method_index(), m_pOwner);
		}
	}
	mu_mutex_locker locker(m_pConnectMethodsMutex);
	for (auto it = m_mapConnectMethods.begin(); it != m_mapConnectMethods.end(); ++it)
	{
		mu_connect_list_ptr pConnectList = (*it).second;
		for (auto c = pConnectList->begin(); c != pConnectList->end(); ++c)
		{
			mu_object_p* pObj_p = mu_object_p::get((*c)->get_obj()->get_p());
			pObj_p->rm_sender(m_pOwner);
		}
	}
}

mu_object_p* mu_object_p::get(void* ptr)
{
	return reinterpret_cast<mu_object_p*>(ptr);
}

bool mu_object_p::is_connected(int signal_relative_index)
{
	mu_mutex_locker locker(m_pConnectMethodsMutex);
	return m_mapConnectMethods.find(signal_relative_index) != m_mapConnectMethods.end();
}

void mu_object_p::add_connect(int signal_relative_index, mu_connect_ptr pConnect)
{
	mu_mutex_locker locker(m_pConnectMethodsMutex);
	if (!is_connected(signal_relative_index))
	{
		m_mapConnectMethods[signal_relative_index] = mu_connect_list_ptr(new mu_connect_list);
	}
	mu_connect_list_ptr pConnectList = m_mapConnectMethods[signal_relative_index];
	pConnectList->push_back(pConnect);
}

mu_connect_list_ptr mu_object_p::get_connect_list(int signal_relative_index)
{
	if (m_mapConnectMethods.find(signal_relative_index) == m_mapConnectMethods.end())
		return nullptr;
	return m_mapConnectMethods[signal_relative_index];
}

void mu_object_p::add_sender(mu_connect_ptr pConnect)
{
	mu_mutex_locker locker(m_pSendersMutex);
	m_listSenders.push_back(pConnect);
}

void mu_object_p::disconnect(int signal_relative_index, mu_object* pObj)
{
	mu_mutex_locker locker(m_pConnectMethodsMutex);
	if (!is_connected(signal_relative_index))
	{
		return;
	}
	mu_connect_list_ptr pConnectList = m_mapConnectMethods[signal_relative_index];
	if (pConnectList->empty())
	{
		return;
	}
	auto it = pConnectList->begin();
	while (it != pConnectList->end())
	{
		if ((*it)->get_obj() == pObj)
		{
			it = pConnectList->erase(it);
			continue;
		}
		++it;
	}
	if (pConnectList->empty())
	{
		m_mapConnectMethods.erase(m_mapConnectMethods.find(signal_relative_index));
	}
}

void mu_object_p::rm_sender(mu_object* pObj)
{
	mu_mutex_locker locker(m_pSendersMutex);
	auto it = m_listSenders.begin();
	while (it != m_listSenders.end())
	{
		if ((*it)->get_obj() == pObj)
		{
			it = m_listSenders.erase(it);
			continue;
		}
		++it;
	}
}

void mu_object_p::set_thread_data(mu_thread_data* pThreadData)
{
	m_pThreadData = pThreadData;
}

mu_thread_data* mu_object_p::get_thread_data()
{
	return m_pThreadData;
}

void mu_object_p::begin_connect_list()
{
	m_pConnectMethodsMutex->lock();
	m_bIsInSignal = true;
}

void mu_object_p::end_connect_list()
{
	m_bIsInSignal = false;
	m_pConnectMethodsMutex->unlock();
}

void mu_object_p::set_delete_later(bool bIsDeleteLater)
{
	m_bIsDeleteLater = bIsDeleteLater;
	if(m_bIsDeleteLater)
	{
		{
			mu_mutex_locker locker(m_pSendersMutex);
			for (auto it = m_listSenders.begin(); it != m_listSenders.end(); ++it)
			{
				mu_object* pSender = (*it)->get_obj();
				mu_object_p* pSenerP = mu_object_p::get(pSender->get_p());
				pSenerP->disconnect((*it)->get_method_index(), m_pOwner);
			}
		}
		mu_mutex_locker locker(m_pConnectMethodsMutex);
		for (auto it = m_mapConnectMethods.begin(); it != m_mapConnectMethods.end(); ++it)
		{
			mu_connect_list_ptr pConnectList = (*it).second;
			for (auto c = pConnectList->begin(); c != pConnectList->end(); ++c)
			{
				mu_object_p* pObj_p = mu_object_p::get((*c)->get_obj()->get_p());
				pObj_p->rm_sender(m_pOwner);
			}
		}
	}
}

bool mu_object_p::is_in_signal()
{
	return m_bIsInSignal;
}

bool mu_object_p::check_delete_later()
{
	bool bIsDeleteLater = m_bIsDeleteLater;
	if(m_bIsDeleteLater)
	{
		m_bIsInSignal = false;
		mu_thread_data* pData = this->get_thread_data();
		pData->decrease_ref();
		delete this;
	}
	return bIsDeleteLater;
}

