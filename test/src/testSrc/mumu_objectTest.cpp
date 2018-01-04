#include <mu_object.h>
#include <gtest/gtest.h>
#include "mumu_objectTest.h"

TEST(mu_object, GeneralConnect)
{
	A testA;
	B testB;
	mu_object::connect(&testA, SIGNAL(send_func(int, char)), &testB, SLOT(recv_func(int, char)));
	testA.send_func(1, 5);
	EXPECT_EQ(1, testB.get_a());
	EXPECT_EQ(5, testB.get_c());
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