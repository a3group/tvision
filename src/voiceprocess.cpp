
#include "../inc/voiceprocess.h"

#include <iostream>

CVoiceProcess::CVoiceProcess()
{
}

CVoiceProcess::~CVoiceProcess()
{
	//data deleted in derived class!
}

CVoiceProcess *CVoiceProcess::processRun(sharedData *psharedData)
{
	volatile int i;
	for (i = 0; i < NCYCLE; i++){};

	psharedData->VVocflg = freeState;

	std::cout << "CVoiceProcess finished" << std::endl;
	return this;
}
