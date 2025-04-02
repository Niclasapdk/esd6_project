// Parameters (Q15 arithmetic)
// maxDelay: maximum delay in samples (15 ms)
// Depth: wet mix (Q15)
// invDepth: dry mix (Q15)
// Delay: delay modulation depth (Q15)
// Rate: LFO rate in mHz
#include "lfo.h"
#include <stdio.h>

Int16 adcvalue = 0;
int maxDelay = 662;       
int Depth = 22938;        // Wet mix in Q15 (example: 0.5)
int invDepth = 16383;     // Dry mix in Q15 (1 - wet mix)
long Delay = 221;     // Base delay in Q15 (221)
int Rate = 1000;          // LFO Rate in mHz

// State variables
int delayIndex = 0;
int fDelayLine[662];      // Delay buffer
int LFOIndex = 100;
int LFOVal = 0;

// Feedback gain in Q15 (choose a value less than 32767 to avoid instability)
// For instance, 8192 corresponds to ~0.25         
int delaySize;
int delayBack;
int delayedSample;
int yn;

void sDelay(Int16 adcvalue){
	Delay = 22 + (((Int32)adcvalue*(331-22))>>10); // 1ms + diff
}

void sRate(Int16 adcvalue){
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
	Depth = 3277 + (((Int32)adcvalue*(32439-3277))>>10);
}

Int16 flanger_IIR(Int16 xn) {
    // Update LFO value occasionally (as in your original code)
    if (LFOIndex >= 9) {  
        LFOVal = LFO();
        LFOIndex = 0;
    } else {
        LFOIndex++;
    }
    
    // Calculate the current delay size (modulated by the LFO)
    delaySize = (maxDelay >> 1) + (((long)Delay * LFOVal) >> 15);
    delayBack = delayIndex - delaySize;
    if (delayBack < 0) {
        delayBack += maxDelay;
    }
    
    // Read the delayed sample from the delay line
    delayedSample = fDelayLine[delayBack];
    
    // Compute the output as a mix of the current input and the delayed sample
    // (invDepth and Depth act as the dry and wet mix factors, respectively)
    yn = (((long)invDepth * xn) + ((long)Depth * delayedSample)) >> 15;
    
    // IIR part: update the delay line with the current input plus the feedback of the delayed sample
    fDelayLine[delayIndex] = yn;
    
    // Update the delay line pointer
    delayIndex++;
    if (delayIndex >= maxDelay) {
        delayIndex = 0;
    }
    
    return yn;
}