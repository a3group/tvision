
#include "../inc/taskprocess.h"

#include <iostream>

CTaskProcess::CTaskProcess()
{
}

CTaskProcess::~CTaskProcess()
{
	//data deleted in derived class!
}

int CTaskProcess::processRun(int data)
{
	return 1;
}
