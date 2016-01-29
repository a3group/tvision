#ifndef _NETPROCESS_H
#define _NETPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"
#include "shareddata.h"

class CNetProcess :public CTaskProcess
{
public:

	CNetProcess();

	~CNetProcess();

	CNetProcess *processRun(sharedData *psharedData);
};

#endif // _NETPROCESS_H
