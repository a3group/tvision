#ifndef _VOICEPROCESS_H
#define _VOICEPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

class CVoiceProcess :public CTaskProcess
{
public:

	CVoiceProcess();

	~CVoiceProcess();

	int processRun(int);
};

#endif // _VOICEPROCESS_H
