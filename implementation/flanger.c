#include <stdio.h>
#include "flanger.h"
#define MAX_DELAY 662           //15ms * 44.1kHz 

static Int16 Mix = 16384;        // Wet mix in Q15 (example: 0.3)
static Int16 invMix = 16384;    // Dry mix in Q15 (1 - wet mix)
static long Delay = 221;        // Base delay (5ms*44.1kHz)
int fDelayLine[MAX_DELAY];      // Delay buffer

/*Parameters for flangerLFO*/
extern long EPM(long *, long *); 

#define LFO_NORMALIZATION 23170 // [-1,1] normalize output
#define MAP_BY_TWO_PI_FS 1530 //2*pi*mapfactor/fs,fs=4410, mapfactor=0.1*2^15/100
#define THREE_FRAC_TWO 1610612736 // 1.5 Q2.30

static long oneMinusKpow2Frac2 = 2147483104;
static long k = 1530000; // the value is 0.0142 but changed to Q1.31

void flangerFRate(Int16 r);

void flangerSetDelay(Int16 adcVal) {
	Delay = 22 + (((Int32)adcVal*(331-22))>>10); // 0.5ms + diff
}

void flangerSetRate(Int16 adcVal) {
	Int16 rate; 	
    if(adcVal > 1000)
	{
		rate = 1000;
	}
	else if(adcVal < 100){
		rate = 100;
	}
	else{
		rate = adcVal;
	}
	flangerFRate(rate); // flangerLFO rate in mHz
}

void flangerSetMix(Int16 adcVal) {	
	// adcVal is 10-bit so result will be 15 bit unsigned (16-bit signed).
	Mix = adcVal*16; // max mix approx. 0.5
    invMix = 32767-Mix;
}

void flangerFRate(Int16 r) {
    k = ((long)MAP_BY_TWO_PI_FS * r) << 1; //Q1.31
    oneMinusKpow2Frac2 = (EPM(&k, &k) >> 1); //k^2/2
    oneMinusKpow2Frac2 = (2147483648 - oneMinusKpow2Frac2); //Q1.31
}

Int16 flangerLFO() {
	Int16 out = 0;
	static long delaylineLFO[2] = {1518270939,0}; //Q1.31 format for 0.707 and 0 
	long agc = 1073741824; // 1 for start gain
	Int16 outTemporary;
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
	outTemporary = _rnd(y1) >> 16;
	out = ((long)outTemporary * LFO_NORMALIZATION) >> 14;
	return out;
}

Int16 flanger_FIR(Int16 xn) {
	static Int16 delayIndex;
	static Int16 LFOIndex = 10;
	static Int16 LFOVal = 0;
	Int16 delaySize;
	Int16 delayBack;
	static Int16 delayedSample;
	Int16 yn;
    // Update flangerLFO value occasionally
    if (LFOIndex >= 9) {  
        LFOVal = flangerLFO();
        LFOIndex = 0;
    } else {
        LFOIndex++;
    }
    
    // Calculate the current delay size (modulated by the flangerLFO)
    delaySize = (MAX_DELAY >> 1) + (((long)Delay * LFOVal) >> 15); // size of delay between 0 and 662
    delayBack = delayIndex - delaySize;
    if (delayBack < 0) {
        delayBack += MAX_DELAY;
    }
    
    // Read the delayed sample from the delay line
    delayedSample = fDelayLine[delayBack];

    // Compute the output as a mix of the current input and the delayed sample
    yn = (((long)invMix * xn) + ((long)Mix * delayedSample)) >> 15;
    
    // Update the delay line with the current input plus the feedback of the delayed sample
    fDelayLine[delayIndex] = xn;
    
    // Update the delay line pointer
    delayIndex++;
    if (delayIndex >= MAX_DELAY) {
        delayIndex = 0;
    }
    return yn;
}
