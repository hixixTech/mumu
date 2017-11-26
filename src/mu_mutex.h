#pragma once
#include <memory>
class mu_mutex
{
public:
	mu_mutex();
	~mu_mutex();
public:
	void lock();
	void unlock();
private:
	void* g_CS;
};
typedef std::shared_ptr<mu_mutex> mu_mutex_ptr;

