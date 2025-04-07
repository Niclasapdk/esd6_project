#ifndef FLANGER_H_
#define FLANGER_H_

#include <ezdsp5535.h>

Int16 flanger_IIR(Int16 xn);
void flangerSetDelay(Int16 adcvalue);
void flangerSetRate(Int16 adcvalue);
void flangerSetMix(Int16 adcvalue);

#endif /*FLANGER_H_*/