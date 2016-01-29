
#include "../inc/vcapprocess.h"

#include <iostream>

CVCapProcess::CVCapProcess()
{
}

CVCapProcess::~CVCapProcess()
{
	//data deleted in derived class!
}

CVCapProcess *CVCapProcess::processRun(sharedData *psharedData)
{
	volatile int i;
	for (i = 0; i < NCYCLE; i++){};

	psharedData->VCapflg = freeState;

	std::cout << "CVCapProcess finished" << std::endl;
	return this;
}
