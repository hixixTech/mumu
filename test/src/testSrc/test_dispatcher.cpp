#include "test_dispatcher.h"

void test_dispatcher::post_event(mu_event* pEvent)
{
	::PostMessage(m_pEventHwnd, MU_NOTIFY_EVENT, (WPARAM)pEvent, (LPARAM)this);
}

test_dispatcher::test_dispatcher(HWND pEventHwnd) :m_pEventHwnd(pEventHwnd)
{

}

