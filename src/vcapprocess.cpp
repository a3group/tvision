
#include "../inc/vcapprocess.h"

#include <iostream>

CVCapProcess::CVCapProcess()
{
}

CVCapProcess::~CVCapProcess()
{
	//data deleted in derived class!
}

int CVCapProcess::processRun(int data)
{
	volatile int i;
	for (i = 0 < 100000; i++;){};
	return 1;
}
