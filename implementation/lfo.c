#include <stdio.h>
#include "lfo.h"

extern long EPM(long *, long *);

long oneMinusKpow2Frac2 = 2147481468;
int mapByTwoPiFs = 1530; //2*pi*mapfactor/fs,fs=4410, mapfactor=0.1*2^15/100;
long delaylineLFO[2] = {1518270939,0}; //Q1.31 format for 0.707 and 0 
long agc = 1073741824; // 1 for start gain
long threefractwo = 1610612736; // 1.5 Q2.30
Int16 lfoNormalization = 23170; // for output normalization [-1,1]
long k = 30596646; // the value is 0.0142 but changed to Q1.31
int rate = 1000;
int out;
int outTemporary;
void fRate(int r)
{
    rate = r;
    k = ((long)mapByTwoPiFs * r) << 1; //Q1.31
    oneMinusKpow2Frac2 = (EPM(&k, &k) >> 1); //k^2/2
    oneMinusKpow2Frac2 = (2147483648 - oneMinusKpow2Frac2); //Q1.31
}
Int16 LFO(){
	long y1 = (EPM(&oneMinusKpow2Frac2, &delaylineLFO[0]) + EPM(&k, &delaylineLFO[1]));
	long y2 = (EPM(&oneMinusKpow2Frac2, &delaylineLFO[1]) - EPM(&k, &delaylineLFO[0]));
	long power = (EPM(&y1, &y1) + EPM(&y2, &y2)) >> 1; //Q2.30, so its possible to fit 1.5 in the
	agc = (threefractwo - power); // Q1.31
	y1 = EPM(&y1, &agc) << 1;
	y2 = EPM(&y2, &agc) << 1;
	/*next iteration*/
	delaylineLFO[0] = y1;
	delaylineLFO[1] = y2;
	/*Output change to match -1 and 1*/
	outTemporary = _rnd(y1) >> 16;
	out = ((long)outTemporary * lfoNormalization) >> 14;
	return out;
}