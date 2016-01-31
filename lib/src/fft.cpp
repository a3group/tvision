
#include "fft.h"

#include <iostream>

CFft::CFft()
{
}

CFft::~CFft()
{
}

void CFft::doFfft(float *x, float *w, int n)
{
	short n2, ie, ia, i, j, k, m;                                    
	float rtemp, itemp, c, s;                                        

	n2 = n;                                                          
	ie = 1;                                                          

	for(k=n; k > 1; k >>= 1)                                         
	{                                                                
		n2 >>= 1;                                                     
		ia = 0;                                                       
		for(j=0; j < ie; j++)                                         
		{                                                             
			c = w[2*j];                                                
			s = w[2*j+1];                                              
			for(i=0; i < n2; i++)                                      
			{                                                          
				m = ia + n2;                                            
				rtemp     = c * x[2*m]   + s * x[2*m+1];                
				itemp     = c * x[2*m+1] - s * x[2*m];                  
				x[2*m]    = x[2*ia]   - rtemp;                          
				x[2*m+1]  = x[2*ia+1] - itemp;                          
				x[2*ia]   = x[2*ia]   + rtemp;                          
				x[2*ia+1] = x[2*ia+1] + itemp;                          
				ia++;                                                   
			}                                                          
			ia += n2;                                                  
		}                                                             
		ie <<= 1;                                                     
	}                                                                
}

void CFft::doIfft(float *x, float *w, int n)
{                                                              
	short n2, ie, ia, i, j, k, m;                              
	float rtemp, itemp, c, s;                                  

	n2 = 1;                                                    
	ie = n;                                                    
	for(k=n; k > 1; k >>= 1)                                   
	{                                                          
		ie >>= 1;                                              
		ia = 0;                                                
		for(j=0; j < ie; j++)                                  
		{                                                      
			c = w[2*j];                                        
			s = w[2*j+1];                                      
			for(i=0; i < n2; i++)                              
			{                                                  
				m = ia + n2;                                   
				rtemp     = x[2*ia]   - x[2*m];                
				x[2*ia]   = x[2*ia]   + x[2*m];                
				itemp     = x[2*ia+1] - x[2*m+1];              
				x[2*ia+1] = x[2*ia+1] + x[2*m+1];              
				x[2*m]    = c*rtemp   - s*itemp;               
				x[2*m+1]  = c*itemp   + s*rtemp;               
				ia++;                                          
			}                                                  
			ia += n2;                                          
		}                                                      
		n2 <<= 1;                                              
	}                                                          
}                                                              

void CFft::doBitrevIdx(short *index, int n)
{
	int   i, j, k, radix = 2;
	short nbits, nbot, ntop, ndiff, n2, raddiv2;

	nbits = 0;
	i = n;
	while (i > 1)
	{
		i = i >> 1;
		nbits++;
	}

	raddiv2 = radix >> 1;
	nbot    = nbits >> raddiv2;
	nbot    = nbot << (raddiv2 - 1);
	ndiff   = nbits & raddiv2;
	ntop    = nbot + ndiff;
	n2      = 1 << ntop;

	index[0] = 0;
	for ( i = 1, j = n2/radix + 1; i < n2 - 1; i++)
	{
		index[i] = j - 1;

		for (k = n2/radix; k*(radix-1) < j; k /= radix)
			j -= k*(radix-1);

		j += k;
	}
	index[n2 - 1] = n2 - 1;
}

int CFft::doGenTwiddle(float *w, int n)
{
	double delta = 2 * M_PI / n;
	int i;
	for(i = 0; i < n/2; i++)
	{
		w[2 * i + 1] = (float)sin(i * delta);
		w[2 * i] = (float)cos(i * delta);
	}

	return n;
}

void CFft::doBitrev(float *w, int n_2)
{
	int i, j, k;
	float rtemp, itemp;

	j = 0;
	for(i=1; i < (n_2-1); i++)
	{
		k = n_2 >> 1;
		while(k <= j)
		{
			j -= k;
			k >>= 1;
		}
		j += k;
		if(i < j)
		{
			rtemp    = w[j*2];
			w[j*2]   = w[i*2];
			w[i*2]   = rtemp;
			itemp    = w[j*2+1];
			w[j*2+1] = w[i*2+1];
			w[i*2+1] = itemp;
		}
	}
}

void CFft::doRescale(float* x, int n)
{
	int i;                                                   
	float inv = (float)(1.0 / (float)n);                                     

	for(i=0; i < n; i++)                                     
	{                                                        
		x[2*i]   = inv * x[2*i];                             
		x[2*i+1] = inv * x[2*i+1];                           
	}                                                        
}
