#include "wah.h"

Int16 F1 = 4665; // Q15;
Int16 Q1 = 3277*2; // Q15: damping * 2

Int16 wah(Int16 x) {
	static Int16 yaux = 0, yl = 0; // Q15
	Int16 yh = x - yl - (((Int32)Q1*yaux)>>15); // Q15
	Int16 y = (((Int32)F1 * yh)>>15) + yaux;
	yl = (((Int32)F1*y)>>15) + yl;
	yaux = y;
	return y;	
}
