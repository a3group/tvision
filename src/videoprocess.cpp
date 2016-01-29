
#include "../inc/videoprocess.h"

#include <iostream>

CVideoProcess::CVideoProcess()
{
}

CVideoProcess::~CVideoProcess()
{
	//data deleted in derived class!
}

CVideoProcess *CVideoProcess::processRun(sharedData *psharedData)
{
	volatile int i;
	for (i = 0; i < NCYCLE; i++){};

	psharedData->VVidflg = freeState;

	std::cout << "CVideoProcess finished" << std::endl;
	return this;
}
