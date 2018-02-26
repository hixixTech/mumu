#include <mu_object.h>
#include <gtest/gtest.h>
#include "test_dispatcher.h"
#include "mumu_objectTest.h"


void A::recv_func(int a, char c)
{
	_a = a;
	_c = c;
}

void A::DeleteTestA()
{
	delete this;
	EXPECT_EQ(true, true);
}

void A::recv_noparams()
{
	_a = 100;
}
void A::recv_CC(C* testC, C& testC1)
{
	EXPECT_EQ(_c1, testC->get_data());
	EXPECT_EQ(_c2, testC1.get_data());
}

void B::recv_func(int a, char c)
{
	_a = a;
	_c = c;
}
void B::DeleteTest()
{
	delete this;
	EXPECT_EQ(true, true);
}
TEST(mu_object, GeneralConnect)
{
	A testA;
	B testB;
	mu_object::connect(&testA, SIGNAL(send_func(int, char)), &testB, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	EXPECT_EQ(1, testB.get_a());
	EXPECT_EQ(5, testB.get_c());
}

TEST(mu_object, DeleteTest)
{
	A* testA = new A;
	B* testB = new B;
	mu_object::connect(testA, SIGNAL(DeleteTest()), testB, SLOT(DeleteTest()));
	testA->DeleteTest();
	delete testA;
}

TEST(mu_object, DeleteSelf)
{
	A* testA = new A;
	mu_object::connect(testA, SIGNAL(DeleteTest()), testA, SLOT(DeleteTestA()));
	testA->DeleteTest();
}

TEST(mu_object, SigIsNull)
{
	A testA;
	B testB;
	mu_object::connect(NULL, SIGNAL(send_func(int, char)), &testB, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	testB.set_a(8);
	testB.set_c(9);
	EXPECT_EQ(8, testB.get_a());
	EXPECT_EQ(9, testB.get_c());
}

TEST(mu_object, ReceiverIsNull)
{
	A testA;
	B testB;
	mu_object::connect(&testA, SIGNAL(send_func(int, char)), NULL, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	testB.set_a(8);
	testB.set_c(9);
	EXPECT_EQ(8, testB.get_a());
	EXPECT_EQ(9, testB.get_c());
}

TEST(mu_object, HaveNoSigMark)
{
	A testA;
	B testB;
	mu_object::connect(&testA, SLOT(send_func(int, char)), &testB, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	EXPECT_NE(1, testB.get_a());
	EXPECT_NE(5, testB.get_c());
}

TEST(mu_object, HaveNoSignal)
{
	A testA;
	B testB;
	mu_object::connect(&testA, SIGNAL(send_func_have_no(int, char)), &testB, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	EXPECT_NE(1, testB.get_a());
	EXPECT_NE(5, testB.get_c());

	mu_object::connect(&testA, SIGNAL(send_func(char)), &testB, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	EXPECT_NE(1, testB.get_a());
	EXPECT_NE(5, testB.get_c());
}

TEST(mu_object, HaveNoSlot)
{
	A testA;
	B testB;

	mu_object::connect(&testA, SIGNAL(send_func(int, char)), &testB, SLOT(recv_func(char)));
	testA.send_func(1, 5);
	EXPECT_NE(1, testB.get_a());
	EXPECT_NE(5, testB.get_c());
}

TEST(mu_object, HaveNoParams)
{
	A testA;
	B testB;

	mu_object::connect(&testB, SIGNAL(send_noparams()), &testA, SLOT(recv_noparams()));
	testB.send_noparams();
	EXPECT_EQ(100, testA.get_a());
}

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

HWND create_test_hwnd(LPCTSTR strClassName)
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
	wndclass.lpszClassName = strClassName;
	if (!RegisterClass(&wndclass))
	{
		return NULL;
	}
	HWND hwnd = CreateWindow(strClassName, "test window",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	return hwnd;
}

DWORD WINAPI threadFunc(PVOID pvparam);

B* test1 = NULL;
A* test2 = NULL;
DWORD nMainThreadID = 0, nThreadId = 0;

TEST(mu_object, MuitiThreadConnect)
{
	test_dispatcher dispatcher(create_test_hwnd("testMain"));
	mu_object::set_dispatcher(&dispatcher);
	nMainThreadID = GetCurrentThreadId();
	test1 = new B;
	HANDLE handle = CreateThread(NULL, 0, threadFunc, 0, 0, &nThreadId);
	dispatcher_msg();
	mu_object::connect(test1, SIGNAL(send_test(int, char)), test2, SLOT(recv_func(int, char)));
	EXPECT_EQ(nThreadId, test1->get_a());
	MU_EMIT test1->send_test(nMainThreadID, 0);
	WaitForSingleObject(handle, INFINITE);
	delete test1;
}

DWORD WINAPI threadFuncDelete(PVOID pvparam);

TEST(mu_object, MuitiThreadDelete)
{
	test1 = new B;
	HANDLE handle = CreateThread(NULL, 0, threadFuncDelete, 0, 0, &nThreadId);

	for (int i = 0; i < 5000; ++i)
	{
		MU_EMIT test1->send_test(5, 6);
	}
	WaitForSingleObject(handle, INFINITE);
	delete test1;
}

DWORD WINAPI threadFuncSelfDefineType(PVOID pvparam);
TEST(mu_object, SelfDefineType)
{
	test_dispatcher dispatcher(create_test_hwnd("testMain1"));
	mu_object::set_dispatcher(&dispatcher);
	test1 = new B;
	C testC, testC1;
	testC.set_data(5);
	testC1.set_data(6);
	HANDLE handle = CreateThread(NULL, 0, threadFuncSelfDefineType, 0, 0, &nThreadId);
	dispatcher_msg();
	mu_object::connect(test1, SIGNAL(send_ccc(C*, C&)), test2, SLOT(recv_CC(C*, C&);));
	EXPECT_EQ(nThreadId, test1->get_a());
	MU_EMIT test1->send_ccc(&testC, testC1);
	WaitForSingleObject(handle, INFINITE);
	delete test1;
}

DWORD WINAPI threadFuncSelfDefineType(PVOID pvparam)
{
	test_dispatcher dispatcher(create_test_hwnd("testThread1"));
	mu_object::set_dispatcher(&dispatcher);
	test2 = new A;
	test2->set_c_data(5, 6);
	nThreadId = GetCurrentThreadId();
	mu_object::connect(test2, SIGNAL(send_func(int, char)), test1, SLOT(recv_func(int, char)));
	MU_EMIT test2->send_func(nThreadId, 6);
	dispatcher_msg();
	delete test2;
	return 0;
}

DWORD WINAPI threadFuncDelete(PVOID pvparam)
{
	test2 = new A;
	for (int i = 0; i < 5000; ++i)
	{
		A* temp = test2;
		test2 = new A;
		mu_object::connect(test1, SIGNAL(send_test(int, char)), test2, SLOT(recv_func(int, char)));
		delete temp;
	}
	delete test2;
	return 0;
}


DWORD WINAPI threadFunc(PVOID pvparam)
{
	test_dispatcher dispatcher(create_test_hwnd("testThread"));
	mu_object::set_dispatcher(&dispatcher);
	test2 = new A;
	nThreadId = GetCurrentThreadId();
	mu_object::connect(test2, SIGNAL(send_func(int, char)), test1, SLOT(recv_func(int, char)));
	MU_EMIT test2->send_func(nThreadId, 6);
	dispatcher_msg();
	EXPECT_EQ(nMainThreadID, test2->get_a());
	delete test2;
	return 0;
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
			::PostQuitMessage(0);
		}
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}



int C::get_data()
{
	return nA;
}

void C::set_data(int a)
{
	nA = a;
}
