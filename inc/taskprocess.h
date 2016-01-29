#ifndef _TASKPROCESS_H
#define _TASKPROCESS_H

#include <iostream>
#include <vector>

#include "shareddata.h"

class CTaskProcess
{
public:

	explicit CTaskProcess();

	virtual ~CTaskProcess();

	virtual CTaskProcess * processRun(sharedData *psharedData);
};

#endif // _TASKPROCESS_H
