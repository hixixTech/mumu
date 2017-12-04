#include "mumu_test.h"
#include "test_dispatcher.h"
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int dispatcher_msg()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

HWND create_test_hwnd()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "yesy";
	if (!RegisterClass(&wndclass))
	{
		return NULL;
	}
	HWND hwnd = CreateWindow("yesy", "test window",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	return hwnd;
}

DWORD WINAPI threadFunc(PVOID pvparam);
mumu_test* test1, *test2;
int main()
{
	test_dispatcher dispatcher(create_test_hwnd());
	mu_object::set_dispatcher(&dispatcher);
	test1 = new mumu_test;
	DWORD threadID = GetCurrentThreadId();
	CreateThread(NULL, 0, threadFunc, 0, 0, &threadID);

	return dispatcher_msg();
}

DWORD WINAPI threadFunc(PVOID pvparam)
{
	test_dispatcher dispatcher(create_test_hwnd());
	mu_object::set_dispatcher(&dispatcher);
	test2 = new mumu_test;
	DWORD threadID = GetCurrentThreadId();
	mu_object::connect(test2, SIGNAL(del_func(int, char)), test1, SLOT(recv_func(int, char)));
	MU_EMIT test2->del_func(5, 6);
	return dispatcher_msg();
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case MU_NOTIFY_EVENT:
		{
								test_dispatcher* dispatcher = reinterpret_cast<test_dispatcher*>(lparam);
								dispatcher->handle_event(reinterpret_cast<mu_event*>(wparam));
								DWORD threadID = GetCurrentThreadId();
								int a;
								a = 5;
		}
			break;
		default:
			break;
	}
	return 1;
}
