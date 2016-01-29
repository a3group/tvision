
#include "../inc/taskprocess.h"

#include <iostream>

CTaskProcess::CTaskProcess()
{
}

CTaskProcess::~CTaskProcess()
{
	//data deleted in derived class!
}

CTaskProcess *CTaskProcess::processRun(sharedData *psharedData)
{
	std::cout << "CTaskProcess finished" << std::endl;

	return this;
}
