#include <ezdsp5535_i2s.h>
#include <ezdsp5535.h>
#include "codec.h"
#include <stdio.h>
extern long EPM(long *, long *);

long oneKpow2 = 2147481468; // 1-k^2/2 initial value
int twopifsmap = 1530; //2*pi*mapfactor/fs where fs = 4410 and mapfactor is 0.1*2^15/100;
long lfoDelayLine[2] = {1518270939,0}; //0.707 in Q1.31 format and 0 
long AGC = 1073741824; // 1 for start gain
long threefractwo = 1610612736; // 1.5 Q2.30
Int16 lfonorm = 23170; // for output normalization [-1,1]
long LFOK = 30596646; // the value is 0.0142 but changed to Q1.31
//int mix = 50;
//int delay = 100;
Int16 rate = 1000;
int out;
int outtmp;
void fRate(int r) //
{
    rate = r;
    LFOK = ((long)twopifsmap * r) << 1; //Q1.31
    oneKpow2 = (EPM(&LFOK, &LFOK) >> 1); //k^2/2
    oneKpow2 = (2147483648 - oneKpow2); //Q1.31
}
Int16 LFO(){
	long y1 = (EPM(&oneKpow2, &lfoDelayLine[0]) + EPM(&LFOK, &lfoDelayLine[1]));
	long y2 = (EPM(&oneKpow2, &lfoDelayLine[1]) - EPM(&LFOK, &lfoDelayLine[0]));
	long p = (EPM(&y1, &y1) + EPM(&y2, &y2)) >> 1; //Q2.30, so its possible to fit 1.5 in the
	AGC = (threefractwo - p); // Q1.31
	/*Adjust of gain*/
	y1 = EPM(&y1, &AGC) << 1;
	y2 = EPM(&y2, &AGC) << 1;
	/*Saving output for next iteration*/
	lfoDelayLine[0] = y1;
	lfoDelayLine[1] = y2;
	/*Between -1 and 1*/
	outtmp = _rnd(y1) >> 16;
	out = ((long)outtmp * lfonorm) >> 14;
	return out;
}
