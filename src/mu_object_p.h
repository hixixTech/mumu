#pragma once
#include <map>
#include "mu_connect.h"
class mu_object;
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
private:
	std::map<int, mu_connect_list_ptr> m_mapConnectMethods;
	mu_object* m_pOwner;
};

