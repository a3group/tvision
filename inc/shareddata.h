#ifndef _SHAREDDATA_H
#define _SHAREDDATA_H

#include <iostream>
#include <vector>

enum taskState{
	freeState = 0,
	busyState,
	needState
};

struct sharedData{
	taskState VCapflg;
	taskState VVidflg;
	taskState VNetflg;
	taskState VVocflg;
	int number;
	float fdata;
};

//debug data
#define NCYCLE 10000000

#endif // _SHAREDDATA_H
