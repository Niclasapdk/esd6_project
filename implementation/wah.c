#include "wah.h"

Int16 F1 = 4665; // Q15
const Int16 Q1 = 2*3277; // Q15 (0.1)

void setWahPedal(Int16 adcVal) {
	const Int16 sin_coeff = 5; // 2*sin(pi/fs) = 0.00014247585718515246 (approx 5 in Q15)
	// fmin + adcVal * fdiff / 1024
	Int16 Fc = 350 + (((Int32)adcVal*(2250-350))>>10); // Q15.0
	// numberrep Q0.15 * Q15.0 = Q15.15
	// actual sizes 12 bit * 3 bit so no need for double precision accumulator
	F1 = sin_coeff*Fc;
}