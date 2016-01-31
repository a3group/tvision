#ifndef _FFTLIBRARY_H
#define _FFTLIBRARY_H

#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

class CFft
{
public:

/**constructor
*/
	CFft();

/**destructor
*/
	~CFft();

/**forward FFT
*/
void doFfft(float *x, float *w, int n);

/**inverse FFT
*/
void doIfft(float *x, float *w, int n);

/**bit reverse indexes
*/
void doBitrevIdx(short *index, int n);

/**butterfly calculation
*/
int doGenTwiddle(float *FftW, int n);

/**bit reverse
*/
void doBitrev(float *FftW, int n_2);

/**divide each element of x by n 
*/
void doRescale(float* x, int n);


private:
short *table;
};

#endif // _FFTLIBRARY_H
