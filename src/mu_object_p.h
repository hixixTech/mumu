#pragma once
#include <map>
#include "mu_connect.h"
class mu_object;
class mu_mutex;
class mu_object_p
{
public:
	mu_object_p(mu_object* pOwner);
	~mu_object_p();
public:
	static mu_object_p* get(void* ptr);
	bool is_connected(int signal_relative_index);
	void add_connect(int signal_relative_index, mu_connect_ptr pConnect);
	mu_connect_list_ptr get_connect_list(int signal_relative_index);
	void add_sender(mu_connect_ptr pConnect);
	void disconnect(int signal_relative_index, mu_object* pObj);
	void rm_sender(mu_object* pObj);
private:
	std::map<int, mu_connect_list_ptr> m_mapConnectMethods;
	mu_object* m_pOwner;
	std::list<mu_connect_ptr> m_listSenders;
	std::shared_ptr<mu_mutex> m_pSendersMutex;
	std::shared_ptr<mu_mutex> m_pConnectMethodsMutex;
};

