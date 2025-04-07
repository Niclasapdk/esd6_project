// Parameters (Q15 arithmetic)
// MAX_DELAY: maximum delay in samples (15 ms)
// Mix: wet mix (Q15)
// invMix: dry mix (Q15)
// Delay: delay modulation depth (Q15)
// Rate: LFO rate in mHz
#include "lfo.h"
#include <stdio.h>

#define MAX_DELAY 662 

static Int16 Mix = 9830;        // Wet mix in Q15 (example: 0.3)
static Int16 invMix = 22937;     // Dry mix in Q15 (1 - wet mix)
static long Delay = 221;     // Base delay in Q15 (221)
// State variables
int fDelayLine[MAX_DELAY];      // Delay buffer


// Feedback gain in Q15 (choose a value less than 32767 to avoid instability)
// For instance, 8192 corresponds to ~0.25         


void flangerSetDelay(Int16 adcvalue){
	Delay = 22 + (((Int32)adcvalue*(331-22))>>10); // 1ms + diff
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
	fRate(rate);
}

void flangerSetMix(Int16 adcvalue){
	Mix = 3277 + (((Int32)adcvalue*(32439-3277))>>10);
}

Int16 flanger_IIR(Int16 xn) {
	static Int16 delayIndex;
	static Int16 LFOIndex = 100;
	static Int16 LFOVal = 0;
	Int16 delaySize;
	Int16 delayBack;
	static Int16 delayedSample;
	Int16 yn;
    // Update LFO value occasionally (as in your original code)
    if (LFOIndex >= 9) {  
        LFOVal = LFO();
        LFOIndex = 0;
    } else {
        LFOIndex++;
    }
    
    // Calculate the current delay size (modulated by the LFO)
    delaySize = (MAX_DELAY >> 1) + (((long)Delay * LFOVal) >> 15);
    delayBack = delayIndex - delaySize;
    if (delayBack < 0) {
        delayBack += MAX_DELAY;
    }
    
    // Read the delayed sample from the delay line
    delayedSample = fDelayLine[delayBack];
    
    // Compute the output as a mix of the current input and the delayed sample
    // (invMix and Mix act as the dry and wet mix factors, respectively)
    yn = (((long)invMix * xn) + ((long)Mix * delayedSample)) >> 15;
    
    // IIR part: update the delay line with the current input plus the feedback of the delayed sample
    fDelayLine[delayIndex] = yn;
    
    // Update the delay line pointer
    delayIndex++;
    if (delayIndex >= MAX_DELAY) {
        delayIndex = 0;
    }
    
    return yn;
}