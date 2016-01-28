#include <iostream>
#include <vector>
#include <array>
#include <initializer_list>
#include "../inc/taskprocess.h"
#include "../inc/vcapprocess.h"
#include "../inc/videoprocess.h"
#include "../inc/voiceprocess.h"
#include "../inc/netprocess.h"

#include "../inc/threadpool.h"

//Number of processor's kernels (need to effictively load processor by creating number of threads)
const int nKernels = 4;
//Number of tasks (not need to be equal number of kernel)
const int nTasks = 4;

using namespace std;

enum taskState{
	free = 0,
	busy,
	need
};

struct staskPri{
	CTaskProcess *pCTaskProcess;
	taskState state;
}

int main(int argc, char **argv)
{
	CVCapProcess VCapProcess;
	CVideoProcess PointsProcess;
	CVoiceProcess VoiceProcess;
	CNetProcess NetProcess;

	vector<staskPri> tasksPri;
	tasksPri.resize(nTasks);
	tasksPri[0].pCTaskProcess = &VCapProcess; tasksPri[0].state = need;
	tasksPri[1].pCTaskProcess = &CVideoProcess; tasksPri[1].state = need;
	tasksPri[2].pCTaskProcess = &CVoiceProcess; tasksPri[2].state = need;
	tasksPri[3].pCTaskProcess = &CNetProcess; tasksPri[3].state = need;

	// create thread pool with 4 worker threads
	ThreadPool poold(nKernels);

	//creating first load at pools
	auto r1 = poold.runAsync<int>([](int i){return i});
	auto r2 = poold.runAsync<int>([](int i){return i});
	auto r3 = poold.runAsync<int>([](int i){return i});
	auto r4 = poold.runAsync<int>([](int i){return i});

/*
	auto r1 = poold.runAsync<int>(&CVideoProcess::processRun, &PointsProcess, 5);
	auto r2 = poold.runAsync<int>(&CVoiceProcess::processRun, &VoiceProcess, 4);
	auto r3 = poold.runAsync<int>(&CVCapProcess::processRun, &VCapProcess, 5);
	auto r4 = poold.runAsync<int>(&CNetProcess::processRun, &NetProcess, 4);
*/

	vector<int> state = {0,0,0,0};

	//while(1){
	//	checking pool size
	//	if(pool.size == FULL){
	//		sleep(1);
	//		continue;
	//	}

	//	for_each(tasksPri.begin, tasksPri.end, [](staskPri &task){
	//		if(task.state == need){
	//			//add to pool if free

	//			task.state = need;
	//		}
	//	})
	//}

	//here get tasks from task's queue
	while(1){
		if (r1->ready){
			state[0] = 1;
		}
		if (r2->ready){
			state[1] = 1;
		}
		if (r3->ready){
			state[2] = 1;
		}
		if (r4->ready){
			state[3] = 1;
		}

		//dispatcher of tasks
		for (int i = 0; i < static_cast<int> (state.size()); i++){
			if (state[i]){
				state[i] = 0;

				if (i == 0){
					r1 = poold.runAsync<int>(&CVideoProcess::processRun, &PointsProcess, 5);
				}
				if (i == 1){
					r2 = poold.runAsync<int>(&CVoiceProcess::processRun, &VoiceProcess, 4);
				}
				if (i == 2){
					r3 = poold.runAsync<int>(&CVCapProcess::processRun, &VCapProcess, 5);
				}
				if (i == 3){
					r4 = poold.runAsync<int>(&CNetProcess::processRun, &NetProcess, 4);
				}
			}
		}
	};

	return 0;
}

