#include "mu_warn.h"
#include <windows.h>


mu_warn::mu_warn()
{
}


mu_warn::~mu_warn()
{
}

void mu_warn::out_put(std::string strWarn)
{
	OutputDebugString(strWarn.data());
}
