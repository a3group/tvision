#include <iostream>
#include <vector>
#include <array>
#include <initializer_list>
#include "../inc/taskprocess.h"
#include "../inc/vcapprocess.h"
#include "../inc/videoprocess.h"
#include "../inc/voiceprocess.h"
#include "../inc/netprocess.h"

#include <windows.h>

#include "../inc/threadpool.h"
#include "../inc/shareddata.h"

using namespace std;

struct staskPri{
	CTaskProcess *pCTaskProcess;
	taskState state;
	taskState *tstate;
};

int main()
{
	//Number of tasks (not need to be equal number of kernel)
	const int nTasks = 4;

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	//Number of processor's kernels (need to effictively load processor by creating number of threads)
	const int nKernels = sysinfo.dwNumberOfProcessors;

	CVCapProcess VCapProcess;
	CVideoProcess VideoProcess;
	CVoiceProcess VoiceProcess;
	CNetProcess NetProcess;

	vector<staskPri> tasksPri;
	tasksPri.resize(nTasks);

	sharedData ssharedData;

	tasksPri[0].pCTaskProcess = &VCapProcess; tasksPri[0].state = needState; tasksPri[0].tstate = &ssharedData.VCapflg;
	tasksPri[1].pCTaskProcess = &VideoProcess; tasksPri[1].state = needState; tasksPri[1].tstate = &ssharedData.VVidflg;
	tasksPri[2].pCTaskProcess = &VoiceProcess; tasksPri[2].state = needState; tasksPri[2].tstate = &ssharedData.VVocflg;
	tasksPri[3].pCTaskProcess = &NetProcess; tasksPri[3].state = needState; tasksPri[3].tstate = &ssharedData.VNetflg;

	//start processes
	ssharedData.VCapflg = needState;
	ssharedData.VVidflg = needState;
	ssharedData.VVocflg = needState;
	ssharedData.VNetflg = needState;

	ThreadPool pool(nKernels);
	std::vector< std::future<int> > results;

	int cycle = 0;
	while (1){
		//add tasks to the pool
		for (int i = 0; i < nTasks; i++) {
			if (*tasksPri[i].tstate == needState){
				*tasksPri[i].tstate = busyState;
				CTaskProcess *pCTaskProcess = tasksPri[i].pCTaskProcess;

				results.emplace_back(
					pool.enqueue(
					[pCTaskProcess, &ssharedData]() {
					pCTaskProcess->processRun(&ssharedData);
					return 0;
				}));
			}
		}

		//check pool's tasks
		while (results.size()){
			for (auto it = results.begin(); it != results.end();)
			{
				if ((*it)._Is_ready()){
					std::cout << cycle++ << "   " << (*it).get() << std::endl;
					results.erase(it);
					it = results.begin();
					continue;
				}
				else{
					it++;
				}
			}
		}

		//start processes (TODO dispatcher of tasks)
		if (ssharedData.VCapflg == freeState) ssharedData.VCapflg = needState;
		if (ssharedData.VVidflg == freeState) ssharedData.VVidflg = needState;
		if (ssharedData.VVocflg == freeState) ssharedData.VVocflg = needState;
		if (ssharedData.VNetflg == freeState) ssharedData.VNetflg = needState;

		Sleep(1);
	}

	//never get here
	while (1){};
}
