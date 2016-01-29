#ifndef _VCAPPROCESS_H
#define _VCAPPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

#include "shareddata.h"

class CVCapProcess :public CTaskProcess
{
public:

	CVCapProcess();

	~CVCapProcess();

	CVCapProcess *processRun(sharedData *psharedData);
};

#endif // _VCAPPROCESS_H
