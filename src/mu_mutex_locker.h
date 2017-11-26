#pragma once
#include <memory>
class mu_mutex;

class mu_mutex_locker
{
public:
	mu_mutex_locker(std::shared_ptr<mu_mutex> pMutex);
	~mu_mutex_locker();
private:
	std::shared_ptr<mu_mutex> m_pMutex;
};

