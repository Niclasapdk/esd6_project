#ifndef FLANGER_H_
#define FLANGER_H_

#include <ezdsp5535.h>

void flangerInit();
Int16 flanger_FIR(Int16 xn);
Int16 flangerChangeDelay(Int16 dir);
Int16 flangerChangeRate(Int16 dir);
Int16 flangerChangeMix(Int16 dir);

#endif /*FLANGER_H_*/
