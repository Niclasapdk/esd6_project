#include "flanger.h"

/* lav til Q15*/
void DepthSet(Int16 Depth, Int16* InvDepth){
    static Int16 Min = 0.001, Max = 0.999, port = 1;        // min = 0.1 % and Max 99.9 %
    parameterSet(Min,Max,port,Depth);                       // Map ADC value for port1 to Depth
    *InvDepth = 1-Depth;                                    // Get InvDepth for x (input)
}
/* lav til Q15 */
void RateSet(Int16 Rate){
    static Int16 Min = 0.1, Max = 1, port = 2;             	// Min 0.1 Hz and Max 1 Hz
    parameterSet(Min,Max,port,Rate);                        // Map ADC value for port2 to Rate
}
/* lav til Q15 */
void DelaySet(Int16 Delay){
    static Int16 Min = 5, Max = 15, port = 3;               // Min 5 Hz and Max 15 Hz for DelayMin
    parameterSet(Min,Max,port,Delay);                    	// Map ADC value for port3 to DelayMin

	/* Måske indsætte width her så den justeres sammen med delay */
   	/* Måske for indsat MaxDelaySamp og MinDelaySamp op her, så den kun skal udregne min og max en gang*/
}


/*Still have to make it into Q15 */
Int16 flangerFx(Int16 x) {
	Static Int16 Delay = 10, Width = 2, Depth = 0.5 , Fs=44100, DelayIndex = 0;


	Int16 minDelaySamp=(((int32)Delay*Fs)>>15)/1000;        //Convert to ms, then samples
	Int16 maxDelaySamp=(((Int32)(Delay + Width)*Fs)>>15)/1000; 	   //Convert to ms, then samples	

	/*lfo shit insert here*/

	ushort DelayLine[DelayIndex] = x;		/*Store 1st element in the delayline to be input*/
	DelayIndex++;							/*Increment, so next input is loaded to next index*/
	if(DelayIndex >= maxDelaySamp) {			/*When the MaxDelay is recieved reset */
		DelayIndex = 0;
	}
	/*Make sure the LFO oscilates in 0 to 1 for this to work*/
	Int16 DelaySize = minDelaySamp + (((Int32)(maxDelaySamp - minDelaySamp ) * LFOValue)>>15);	// Calculate delay used based on lfo
	Int16 DelayBack = DelayIndex - Delaysize;										// Indicates which index the delay is at
																					// EX: 0 - 25 then look at -25 sample
	if (delayBack < 0) {															
    	delayBack += maxDelaySamp;														// Wrap around if negative 
																					// EX: -25 + Max = DelayBack
	}
	/*Mix the modulation effect and the input*/
	x=(((Int32)(1-Depth)*x)>>15) + (((Int32)Depth*DelayLine[DelayBack])>>15);
	return x;
}