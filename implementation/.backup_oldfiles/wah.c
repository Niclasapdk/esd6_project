#include "wah.h"

Int16 F1 = 4665; // Q15;
Int16 Q1 = 3277*2; // Q15: damping * 2


void FcSet(Int16 Fc, Int16 *F1){
	// Needs to be converted to Q15 i think
    static Int16 Min = 350, Max = 2250, port = 1;        // min = 350 hz and Max 2250 Hz
    parameterSet(Min,Max,port,Fc);                       // Map ADC value for port1 to Depth
    *F1 = (EPM(5 * Fc));							 	 // F1=2*sin(pi*Fc/Fs) Q15
	// Sin calculation without its compleksity			     
	// 2*sin(pi/44100) = 0.00014248, Q15 =4.66786, approx 5
	// 0.00014248 * 300 = 0.04274400
	// F1=2*sin(pi*300/44100) = 0.042740
	// almost same result in the coefficient so with much less complexity
}

// Function with pot implemented 
Int16 wahFx(Int16 x) {
	static Int16 yaux = 0, yl = 0, Fc=0 ; // Q15
	FcSet(Fc,&F1);
	Int16 yh = x - yl - ((EPM(Q1*yaux))>>15); // Q15
	Int16 y = ((EPM(&F1 * yh))>>15) + yaux;
	yl = ((EPM(&F1*y))>>15) + yl;
	yaux = y;
	return y;	
}

// Function with constant Fc
Int16 wah(Int16 x) {
	static Int16 yaux = 0, yl = 0; // Q15
	Int16 yh = x - yl - (((Int32)Q1*yaux)>>15); // Q15
	Int16 y = (((Int32)F1 * yh)>>15) + yaux;
	yl = (((Int32)F1*y)>>15) + yl;
	yaux = y;
	return y;	
}
