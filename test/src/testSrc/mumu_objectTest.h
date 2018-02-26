#pragma once

#include <mu_object.h>

class C
{
public:
	int get_data();
	void set_data(int a);
private:
	int nA;
};

class A : public mu_object
{
	MU_OBJECT
MU_SIGNALS:
	void send_func(int a, char c);
	void DeleteTest();
public MU_SLOTS:
	void recv_func(int a, char c);
	void recv_CC(C* testC, C& testC1);
	void recv_noparams();
	void DeleteTestA();
public:
	int get_a()
	{
		return _a;
	}
	int get_c()
	{
		return _c;
	}
	void set_c_data(int c1,int c2)
	{
		_c1 = c1;
		_c2 = c2;
	}
private:
	int _a;
	char _c;
	int _c1;
	int _c2;
};

class B :public mu_object
{
	MU_OBJECT
public MU_SLOTS:
	void recv_func(int a, char c);
	void DeleteTest();
public:
	int get_a()
	{
		return _a;
	}
	int get_c()
	{
		return _c;
	}
	void set_a(int a)
	{
		_a = a;
	}
	void set_c(int c)
	{
		_c = c;
	}
MU_SIGNALS:
	void send_test(int a, char c);
	void send_ccc(C* testC, C& testC1);
	void send_noparams();
private:
	int _a;
	char _c;
};