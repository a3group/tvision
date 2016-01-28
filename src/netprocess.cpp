
#include "../inc/netprocess.h"

#include <iostream>

CNetProcess::CNetProcess()
{
}

CNetProcess::~CNetProcess()
{
	//data deleted in derived class!
}

int CNetProcess::processRun(int data)
{
	volatile int i;
	for (i = 0 < 100000; i++;){};
	return 1;
}
