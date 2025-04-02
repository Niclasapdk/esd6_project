#ifndef FLANGER_H_
#define FLANGER_H_

#include <ezdsp5535.h>

Int16 flanger_IIR(Int16 xn);
void sDelay(Int16 adcvalue);
void sRate(Int16 adcvalue);
void sDepth(Int16 adcvalue);

#endif /*FLANGER_H_*/