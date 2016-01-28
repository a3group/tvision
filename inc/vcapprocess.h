#ifndef _VCAPPROCESS_H
#define _VCAPPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

class CVCapProcess :public CTaskProcess
{
public:

	CVCapProcess();

	~CVCapProcess();

	int processRun(int);
};

#endif // _VCAPPROCESS_H
