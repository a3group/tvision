
#include "../inc/netprocess.h"

#include <iostream>

CNetProcess::CNetProcess()
{
}

CNetProcess::~CNetProcess()
{
	//data deleted in derived class!
}

CNetProcess *CNetProcess::processRun(sharedData *psharedData)
{
	volatile int i;
	for (i = 0; i < NCYCLE;i++){};

	psharedData->VNetflg = freeState;

	std::cout << "CNetProcess finished" << std::endl;
	return this;
}
