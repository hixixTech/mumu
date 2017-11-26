#include "mu_connect.h"


mu_connect::mu_connect(mu_metaobject* pMetaObj, mu_object* pObj, int nMethodIndex) :m_pMetaObj(pMetaObj),
m_nMethodIndex(nMethodIndex), m_pObj(pObj)
{
}


mu_connect::~mu_connect()
{
}

int mu_connect::get_method_index()
{
	return m_nMethodIndex;
}

mu_metaobject* mu_connect::get_metaobj()
{
	return m_pMetaObj;
}

mu_object* mu_connect::get_obj()
{
	return m_pObj;
}
