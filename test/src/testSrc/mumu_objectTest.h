#pragma once

#include <mu_object.h>


class A : public mu_object
{
	MU_OBJECT
MU_SIGNALS:
	void send_func(int a, char c);
};

class B :public mu_object
{
	MU_OBJECT
public MU_SLOTS:
	void recv_func(int a, char c)
	{
		_a = a;
		_c = c;
	}
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
private:
	int _a;
	char _c;
};