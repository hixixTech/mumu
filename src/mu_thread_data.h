#pragma once
#include<memory>
#include<atomic>
#include "mu_mutex.h"

class mu_type;
struct mu_metaobject;
class mu_event_dispatcher;
class mu_object;

class mu_event
{
public:
	mu_type* get_type();
	void set_type(mu_type* pType);
	mu_metaobject* get_metaobj();
	void set_metaobj(mu_metaobject* pMetaObj);
	int get_method_index();
	void set_method_index(int nMethodIndex);
	void set_receiver(mu_object* pReceiver);
	mu_object* get_receiver();
private:
	mu_type* m_pType;
	mu_metaobject* m_pMetaObj;
	int m_nMethodIndex;
	mu_object* m_pReceiver;
};
class mu_thread_data
{
public:
	mu_thread_data();
	~mu_thread_data();
public:
	static mu_thread_data* get_current();
	void add_ref();
	void decrease_ref();
	void set_thread_id(int nThreadId);
	unsigned int get_thread_id();
	void set_event_dispatcher(mu_event_dispatcher* pDispatcher);
	mu_event_dispatcher* get_event_dispatcher();
private:
	std::atomic<int> m_ref;
	static mu_mutex_ptr g_pMutex;
	unsigned int m_nThreadId;
	mu_event_dispatcher* m_pDispatcher;
};

