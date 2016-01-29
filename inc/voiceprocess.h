#ifndef _VOICEPROCESS_H
#define _VOICEPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

#include "shareddata.h"

class CVoiceProcess :public CTaskProcess
{
public:

	CVoiceProcess();

	~CVoiceProcess();

	CVoiceProcess *processRun(sharedData *psharedData);
};

#endif // _VOICEPROCESS_H
