// :)
#include "tremolo.h"
#include "lfo.h"

Int16 rate = 1000; //range (100-10000mHz)
Int16 depth = 10000; //range (3277 - 32439) Q15 (3277 = 0.1, 32439 = 0.99) 
Int16 lfoIndex = 100;
Int16 lfoValue = 0;  

void setRateTremolo(Int16 adcVal){
	rate = 100 + (((Int32)adcVal * (10000-100)) >> 10); //Q15.0
}

void setDepthTremolo(Int16 adcVal2){
	depth = 3277 + (((Int32)adcVal2 * (32439 - 3277)) >> 10); //Q0.15
}


Int16 tremolo(Int16 x){

const Int16 initVal = 32767; // 32767 = 1 Q0.15
setDepthTremolo(adcVal2);

if(lfoIndex >= 9){
	setRateTremolo(adcVal);
	fRate(rate);
	lfoValue = lfo(); //Change to lfo name.	
	lfoIndex = 0;
} else {
	lfoIndex++;
}

Int16 TremoloFx = initVal+(((Int32)depth * lfoValue)>>15);		//Q15 Tremolo value effect
x = (((Int32)x * TremoloFx)>>15);								//Q15 Modulated signal with tremolo effect

return x;

}
















