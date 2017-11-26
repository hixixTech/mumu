#pragma once
#include <memory>
#include <list>
struct mu_metaobject;
class mu_object;

class mu_connect
{
public:
	mu_connect(mu_metaobject* pMetaObj, mu_object* pObj, int nMethodIndex);
	~mu_connect();
	int get_method_index();
	mu_metaobject* get_metaobj();
	mu_object* get_obj();
private:
	mu_metaobject* m_pMetaObj;
	mu_object* m_pObj;
	int m_nMethodIndex;
};

typedef std::shared_ptr<mu_connect> mu_connect_ptr;

typedef std::list<mu_connect_ptr> mu_connect_list;

typedef std::shared_ptr<mu_connect_list> mu_connect_list_ptr;
