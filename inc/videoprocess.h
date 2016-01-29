#ifndef _VIDEOPROCESS_H
#define _VIDEOPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

#include "shareddata.h"

class CVideoProcess :public CTaskProcess
{
public:

	CVideoProcess();

	~CVideoProcess();

	CVideoProcess *processRun(sharedData *psharedData);
};

#endif // _VIDEOPROCESS_H
