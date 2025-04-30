#include "wah.h"

Int16 F1 = 4665; // Q15, Fc = 1000
const Int16 Q1 = 2*1704; // Q15 (0.1) * 2, meaning damp *2 = Q1

void setWahPedal(Int16 adcVal) {
	const Int16 sin_coeff = 5; // 2*sin(pi/fs) = 0.00014247585718515246 (approx 5 in Q15)
	// fmin + adcVal * fdiff / 1024
	// range = 350 - 2250 Hz, from 4665/1000 * Hz
	Int16 Fc = 326 + (((Int32)adcVal*(2100-350))>>10); // Q15.0
	// numberrep Q0.15 * Q15.0 = Q15.15
	// actual sizes 12 bit * 3 bit so no need for double precision accumulator
	F1 = sin_coeff*Fc;
}
Int16 wah(Int16 x) {
	static Int16 yaux = 0, yl = 0, y = 0, yh = 0; // Q15
	x = x >> 3;
	yh = x - yl - (((Int32)Q1*yaux)>>15); // Q15
	y = (((Int32)F1 * yh)>>15) + yaux;
	yl = (((Int32)F1*y)>>15) + yl;
	yaux = y;
	return y;	
}