#pragma once
#include "mu_object.h"

class mumu_test : public mu_object
{
	MU_OBJECT
public:
	mumu_test();
	~mumu_test();
MU_SIGNALS:
	void send_func(int a, char c);
	void add_func();
	void del_func(int a, char c);
public MU_SLOTS:
	void recv_func(int a, char c);
};


class mumu_test_111 :public mumu_test
{
	MU_OBJECT
MU_SIGNALS:
	void send_func2(int a, char c);
	void add_func3();
	void del_func4(int a, char c);
};
