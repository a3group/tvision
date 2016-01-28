#ifndef _NETPROCESS_H
#define _NETPROCESS_H

#include <iostream>
#include <vector>

#include "taskprocess.h"

class CNetProcess :public CTaskProcess
{
public:

	CNetProcess();

	~CNetProcess();

	int processRun(int);
};

#endif // _NETPROCESS_H
