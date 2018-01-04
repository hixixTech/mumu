#include "mu_object.h"
#include <windows.h>

#define MU_NOTIFY_EVENT WM_USER + 1

class test_dispatcher :public mu_event_dispatcher
{
public:
	test_dispatcher(HWND pEventHwnd);

public:
	virtual void post_event(mu_event* pEvent);
private:
	HWND m_pEventHwnd;
};