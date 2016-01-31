#if 0
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
#endif


#include "ffft/FFTRealFixLen.h"

#define _USE_MATH_DEFINES
//#include <cmath>
#include <math.h>

#include "fft.h"

#define FFTSIZE 3
#define FFTLEN (1<<FFTSIZE)

#define OTHER_FFFT_LIB

#include "../inc/netprocess.h"

int main()
{
#ifdef OTHER_FFFT_LIB

	ffft::FFTRealFixLen <FFTSIZE> fft_object;

   float x [(FFTLEN<<1)];
   float f [(FFTLEN<<1)];
   for(int i=0;i< FFTLEN*2;i++){
	   x[i]=0.0;
	   f[i]=0.0;
   }

   for(int i=0;i< FFTLEN;i++){
	   x[i] = (float)sinf((float)2.0f*M_PI*0.1f*i);
   }

	for(int k=0;k<10000;k++){
		fft_object.do_fft (f, x);     // x (real) --FFT---> f (complex)
		fft_object.do_ifft (f, x);    // f (complex) --IFFT--> x (real)
		fft_object.rescale (x);       // Post-scaling should be done after FFT+IFFT
	}
#else
	CFft fftobj;

	short table[32];
	float FftW[FFTLEN*2];
	float realData[FFTLEN];
	float complexData[(FFTLEN<<1)];

	fftobj.doBitrevIdx(table, FFTLEN);
	fftobj.doGenTwiddle(FftW, FFTLEN);
	fftobj.doBitrev(FftW, (FFTLEN>>1));

	for(int i=0;i<FFTLEN;i++){
		complexData[i*2+0] = sin((float)(2.0*M_PI*0.1*i));
		complexData[i*2+1] = 0.0f;
	}

	for(int k=0;k<10000;k++){
		fftobj.doFfft(complexData, FftW, FFTLEN);	//forward transform
		fftobj.doIfft(complexData, FftW, FFTLEN);	//Inverse transform
		fftobj.doRescale(complexData, FFTLEN);		//rescale data
	}
#endif


	msg_poll();

	return 0;
}