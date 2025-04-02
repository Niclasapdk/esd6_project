// :)
#include "tremolo.h"
#include "lfo.h"
//range (100-10000mHz)

Int16 depth = 16383; //range (3277 - 32439) Q15 (3277 = 0.1, 16383 = 0.5)Q14 
Int16 lfoIndex = 100;
Int16 lfoValue = 0;
Int16 TremoloFx = 0;
Int16 y = 0;

//void setRateTremolo(Int16 adcVal){
//	rate = 100 + (((Int32)adcVal * (10000-100)) >> 10); //Q15.0
//}
//
//void setDepthTremolo(Int16 adcVal2){
//	depth = 3277 + (((Int32)adcVal2 * (16383 - 3277)) >> 10); //Q0.15
//}


Int16 tremolo(Int16 x){

const Int16 initVal = 16383; // 32767 = 1 Q0.15

if(lfoIndex >= 9){
	lfoValue = LFO(); //Change to lfo name.	
	lfoIndex = 0;
} else {
	lfoIndex++;
}

TremoloFx = (initVal+((Int32)depth * lfoValue)>>15); // Q15 Tremolo value effect
y = (((Int32)x * TremoloFx)>>15); // Q15 Modulated signal with tremolo effect


return y;

}