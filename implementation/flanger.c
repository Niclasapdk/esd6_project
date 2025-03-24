#include "flanger.h"

/*Still have to make it into Q15 */
Int16 flangerFx(Int16 x) {
	Static Int16 Delay = 10, Width = 2, Fs=44100, DelayIndex = 0;


	Int16 minDelaySamp=((((int32)Delay * Fs)*Fs)>>15)/1000;        //Convert to ms, then samples
	Int16 maxDelaySamp=((((Int32)Delay + Width)*Fs)>>15)/1000; 	   //Convert to ms, then samples	

	/*lfo shit insert here*/

	ushort DelayLine[DelayIndex] = x;		/*Store 1st element in the delayline to be input*/
	DelayIndex++;							/*Increment, so next input is loaded to next index*/
	if(DelayIndex >= DelayMax) {			/*When the MaxDelay is recieved reset */
		DelayIndex = 0;
	}
	
	int delaySize = (maxDelay >> 1) + (((Int32)Delay LFOValue)>>15);


	int delayBack = delayIndex - delaySize;
	if (delayBack < 0) {
    	delayBack += maxDelay;
	}
	int yn = (((long)invDepth * xn) + ((long)Depth * fDelayLine[delayBack]) >> 15);




	return x;
}