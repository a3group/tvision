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

using namespace std;

int main(int argc, char **argv)
{
	CVideoProcess PointsProcess;
	CVoiceProcess VoiceProcess;
	CVCapProcess VCapProcess;
	CNetProcess NetProcess;

	// create thread pool with 4 worker threads
	ThreadPool poold(nKernels);

	auto r1 = poold.runAsync<int>(&CVideoProcess::processRun, &PointsProcess, 5);

	auto r2 = poold.runAsync<int>(&CVoiceProcess::processRun, &VoiceProcess, 4);
	auto r3 = poold.runAsync<int>(&CVCapProcess::processRun, &VCapProcess, 5);
	auto r4 = poold.runAsync<int>(&CNetProcess::processRun, &NetProcess, 4);

	vector<int> state = {0,0,0,0};

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

