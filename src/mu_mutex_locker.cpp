#include "mu_mutex_locker.h"
#include "mu_mutex.h"

mu_mutex_locker::mu_mutex_locker(mu_mutex_ptr pMutex) :m_pMutex(pMutex)
{
	m_pMutex->lock();
}


mu_mutex_locker::~mu_mutex_locker()
{
	m_pMutex->unlock();
}
