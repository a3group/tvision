#ifndef _TASKPROCESS_H
#define _TASKPROCESS_H

#include <iostream>
#include <vector>

class CTaskProcess
{
public:

	explicit CTaskProcess();

	virtual ~CTaskProcess();

	virtual int processRun(int);
};

#endif // _TASKPROCESS_H
