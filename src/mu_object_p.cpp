#include "mu_object_p.h"


mu_object_p::mu_object_p(mu_object* pOwner) :m_pOwner(pOwner)
{
}


mu_object_p::~mu_object_p()
{
}

mu_object_p* mu_object_p::get(void* ptr)
{
	return reinterpret_cast<mu_object_p*>(ptr);
}

bool mu_object_p::is_connected(int signal_relative_index)
{
	return m_mapConnectMethods.find(signal_relative_index) != m_mapConnectMethods.end();
}

void mu_object_p::add_connect(int signal_relative_index, mu_connect_ptr pConnect)
{
	if (!is_connected(signal_relative_index))
	{
		m_mapConnectMethods[signal_relative_index] = mu_connect_list_ptr(new mu_connect_list);
	}
	mu_connect_list_ptr pConnectList = m_mapConnectMethods[signal_relative_index];
	pConnectList->push_back(pConnect);
}

mu_connect_list_ptr mu_object_p::get_connect_list(int signal_relative_index)
{
	return m_mapConnectMethods[signal_relative_index];
}

