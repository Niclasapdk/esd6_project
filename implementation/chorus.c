// Parameters (Q15 arithmetic)
// maxDelay: maximum delay in samples (15 ms)
// Mix: wet mix (Q15)
// invMix: dry mix (Q15)
// Delay: delay modulation depth (Q15)
// Rate: LFO rate in mHz
#include <stdio.h>
#include "chorus.h"
extern long EPM(long *, long *); 

#define LFO_NORMALIZATION 23170 // [-1,1] NORMALIZE OUTPUT???
#define MAP_BY_TWO_PI_FS 1530 //2*pi*mapfactor/fs,fs=4410, mapfactor=0.1*2^15/100
#define THREE_FRAC_TWO 1610612736 // 1.5 Q2.30
#define CHORUS_DELAYLINE_LEN 2205

static long oneMinusKpow2Frac2 = 2147481468;
static long delaylineLFO[2] = {1518270939,0}; //Q1.31 format for 0.707 and 0 
static long k = 30596646; // the value is 0.0142 but changed to Q1.31

static Int16 Mix = 10000;        // Wet mix in Q15
static Int16 invMix = 22767;     // Dry mix in Q15 (1 - wet mix)
static Int16 Delay = 1102; // Base delay
static Int16 Width = 221;  // Modulation delay width

// State variables
int cDelayLine[CHORUS_DELAYLINE_LEN] = {0};      // Delay buffer
void sDelay(Int16 adcvalue){
	Delay = 22 + (((Int32)adcvalue*(331-22))>>10); // 1ms + diff
}

void sRate(Int16 adcvalue){
	static int Rate = 500;          // LFO Rate in mHz
	if(adcvalue > 1000)
	{
		Rate = 1000;
	}
	else if(adcvalue < 100){
		Rate = 100;
	}
	else{
		Rate = adcvalue;
	}
}

void sDepth(Int16 adcvalue){
	Mix = 3277 + (((Int32)adcvalue*(32439-3277))>>10);
}

void chorusFRate(Int16 r)
{
    k = ((long)MAP_BY_TWO_PI_FS * r) << 1; //Q1.31
    oneMinusKpow2Frac2 = (EPM(&k, &k) >> 1); //k^2/2
    oneMinusKpow2Frac2 = (2147483648 - oneMinusKpow2Frac2); //Q1.31
}
Int16 chorusLFO(int *out){
	long agc = 1073741824; // 1 for start gain
	int outtmp[2];
	long y1 = (EPM(&oneMinusKpow2Frac2, &delaylineLFO[0]) + EPM(&k, &delaylineLFO[1]));
	long y2 = (EPM(&oneMinusKpow2Frac2, &delaylineLFO[1]) - EPM(&k, &delaylineLFO[0]));
	long power = (EPM(&y1, &y1) + EPM(&y2, &y2)) >> 1; //Q2.30, so its possible to fit 1.5 in the
	agc = (THREE_FRAC_TWO - power); // Q1.31
	y1 = EPM(&y1, &agc) << 1;
	y2 = EPM(&y2, &agc) << 1;
	/*next iteration*/
	delaylineLFO[0] = y1;
	delaylineLFO[1] = y2;
	/*Output change to match -1 and 1*/
    outtmp[0] = _rnd(y1) >> 16;      // sin
    outtmp[1] = _rnd(y2) >> 16;     // cos
    out[0] = ((long)outtmp[0] * LFO_NORMALIZATION) >> 14;
    out[1] = ((long)outtmp[1] * LFO_NORMALIZATION) >> 14;
}

Int16 chorus(Int16 xn) {
	Int16 yn, i, mod;
	static int delayIndex;
	static int LFOIndex = 100;
	static int delaySize[4] = {0};
	static int delayBack[4] = {0};
	static int delayedSample[4] = {0};
	static int LFOVal[4];
    // Update LFO value occasionally (as in your original code)
    if (LFOIndex >= 9) {  
        chorusLFO(LFOVal);
        LFOVal[2]=-LFOVal[0];
        LFOVal[3]=-LFOVal[1];
        LFOIndex = 0;
    } else {
        LFOIndex++;
    }
    for (i = 0; i < 3; i++){
    	 // Calculate the current delay size (modulated by the LFO)
	    delaySize[i] = Delay + (((Int32)Width * LFOVal[i]) >> 15);
	    delayBack[i] = delayIndex - delaySize[i];
	    if (delayBack[i] < 0) {
	        delayBack[i] += CHORUS_DELAYLINE_LEN;
	    }
	    
	    // Read the delayed sample from the delay line
	    delayedSample[i] = cDelayLine[delayBack[i]];
    	
    }
    // Compute the output as a mix of the current input and the delayed sample
    // (invMix and Mix act as the dry and wet mix factors, respectively)
    // For instance, 8192 corresponds to ~0.25
    mod = (((long)delayedSample[0] * 8191) + ((long)delayedSample[1] * 8191) + ((long)delayedSample[2] * 8191) + ((long)delayedSample[3] * 8191)) >> 15;
    yn = (((long)invMix * xn) + ((long)Mix * mod)) >> 15;
    
    // IIR part: update the delay line with the current input plus the feedback of the delayed sample
    cDelayLine[delayIndex] = xn;
    
    // Update the delay line pointer
    delayIndex++;
    if (delayIndex >= CHORUS_DELAYLINE_LEN) {
        delayIndex = 0;
    }
    
    return yn;
}