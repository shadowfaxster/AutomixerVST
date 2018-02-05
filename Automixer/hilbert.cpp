/*//use FFT => Phase Shift => IFFT method to do the hilbert transform 
#include<iostream>
#include <complex>
#include "./fftw3/fftw3.h"
using namespace std;

int main()
{
int N=4; // length of FFT
fftw_complex *in,*out, *in2; // array for I/O

double z;
cout << "Size of fftw_complex: " << sizeof(fftw_complex) << endl;
in = (fftw_complex*) fftw_malloc(N * sizeof(fftw_complex)); // for Inputting
in2 = (fftw_complex*) fftw_malloc(N * sizeof(fftw_complex)); // for output
out = (fftw_complex*) fftw_malloc(N * sizeof(fftw_complex)); // for comparison

// Initializing the arry will be real numbers {1 2 3 4}
for(int i = 0; i < N; i++){
   in[i][0] = i;
   in[i][1] = 0;
   cout<<in[i][0]<<endl;
}



// making a plan and execute it, then destroy it. Forward FFT
fftw_plan pf = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
fftw_execute(pf);
fftw_destroy_plan(pf);
//do hilbert transform do a phase shift f(w)*-i (w<0)
//                                      f(w)*i(w>0)
//                                      f(w)*0(w=0)
for(int i=0;i<N;i++)
{
cout<<out[i][0]<<" "<<out[i][1]<<endl;
}
for(int i= 0; i < N; i++)
{
	
	if(i<N/2)
	{
		
			out[i][1]=-1*out[i][1];

			
		    
	        	
	}
	if(i==N/2)
	{ 
		  out[i][0]=0;
	      out[i][1]=0;


		
	}
	
	if(i>N/2)
	{
		out[i][0]=-1*out[i][0];
	
	
	}
if(i==0)
	{ 
		  out[i][0]=0;
	      out[i][1]=0;


		
	}

            z=out[i][1];
			out[i][1]=out[i][0];
			out[i][0]=z;



}

// making a plan and execute it, then destroy it. iFFT
fftw_plan pb = fftw_plan_dft_1d(N, out, in2, FFTW_BACKWARD, FFTW_ESTIMATE);
fftw_execute(pb);
fftw_destroy_plan(pb);

// output the fore N results
for(int i= 0; i < N; i++){
   cout << in2[i][0]/N << " " <<in2[i][0]/N << endl;
   
}

// free space
fftw_free(in);
fftw_free(in2);
fftw_free(out);
system("pause");
return 0;
}
*/