#ifndef _VIDEOPROCESS_H
#define _VIDEOPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

class CVideoProcess :public CTaskProcess
{
public:

	CVideoProcess();

	~CVideoProcess();

	int processRun(int);
};

#endif // _VIDEOPROCESS_H
