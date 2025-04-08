#include "lfo.h"
#include <stdio.h>

#define MAX_DELAY 662           //15ms * 44.1kHz 

static Int16 Mix = 9830;        // Wet mix in Q15 (example: 0.3)
static Int16 invMix = 22937;    // Dry mix in Q15 (1 - wet mix)
static long Delay = 221;        // Base delay (5ms*44.1kHz)
int fDelayLine[MAX_DELAY];      // Delay buffer

void flangerSetDelay(Int16 adcvalue){
	Delay = 22 + (((Int32)adcvalue*(331-22))>>10); // 0.5ms + diff
}

void flangerSetRate(Int16 adcvalue){
	Int16 rate; 	
    if(adcvalue > 1000)
	{
		rate = 1000;
	}
	else if(adcvalue < 100){
		rate = 100;
	}
	else{
		rate = adcvalue;
	}
	fRate(rate); // LFO rate in mHz
}

void flangerSetMix(Int16 adcvalue){
	Mix = 3277 + (((Int32)adcvalue*(32439-3277))>>10); // 0.1 + diff
    invMix = 32767-Mix;
}

Int16 flanger_IIR(Int16 xn) {
	static Int16 delayIndex;
	static Int16 LFOIndex = 10;
	static Int16 LFOVal = 0;
	Int16 delaySize;
	Int16 delayBack;
	static Int16 delayedSample;
	Int16 yn;
    // Update LFO value occasionally
    if (LFOIndex >= 9) {  
        LFOVal = LFO();
        LFOIndex = 0;
    } else {
        LFOIndex++;
    }
    
    // Calculate the current delay size (modulated by the LFO)
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
    fDelayLine[delayIndex] = yn;
    
    // Update the delay line pointer
    delayIndex++;
    if (delayIndex >= MAX_DELAY) {
        delayIndex = 0;
    }
    
    return yn;
}