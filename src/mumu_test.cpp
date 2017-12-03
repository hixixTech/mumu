#include "mumu_test.h"


mumu_test::mumu_test()
{
	connect(this, SIGNAL(del_func(int, char)), this, SLOT(recv_func(int, char)), ConnectionType::QueuedConnection);
	MU_EMIT del_func(5, 6);
}


mumu_test::~mumu_test()
{
}

void mumu_test::recv_func(int a, char c)
{
	int k;
	k = 5;
}
