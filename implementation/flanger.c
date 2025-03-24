#include "flanger.h"

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