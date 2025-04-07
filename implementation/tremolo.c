#include "tremolo.h"
#include "lfo.h"
//range (100-10000mHz)

static Int16 Mix = 16383; //range (3277 - 32439) Q15 (3277 = 0.1, 16383 = 0.5)Q14 

void tremoloSetRate(Int16 adcvalue){
	Int16 rate;
	rate = 100 + (((Int32)adcvalue * (10000-100)) >> 10); //Q15.0
	fRate(rate); 
}

void tremoloSetMix(Int16 adcvalue){
	Mix = 3277 + (((Int32)adcvalue * (16383 - 3277)) >> 10); //Q0.15
}


Int16 tremolo(Int16 x){
static Int16 lfoIndex = 100;
static Int16 lfoValue = 0;
Int16 TremoloFx = 0,y = 0;
const Int16 initVal = 16383; // 32767 = 1 Q0.15

if(lfoIndex >= 9){
	lfoValue = LFO(); //Change to lfo name.	
	lfoIndex = 0;
} else {
	lfoIndex++;
}

TremoloFx = (initVal+((Int32)Mix * lfoValue)>>15); // Q15 Tremolo value effect
y = (((Int32)x * TremoloFx)>>15); // Q15 Modulated signal with tremolo effect
return y;
}