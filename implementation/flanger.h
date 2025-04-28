#ifndef FLANGER_H_
#define FLANGER_H_

#include <ezdsp5535.h>

Int16 flanger_FIR(Int16 xn);
void flangerChangeDelay(Int16 dir);
void flangerChangeRate(Int16 dir);
void flangerChangeMix(Int16 dir);

#endif /*FLANGER_H_*/
