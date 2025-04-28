#ifndef CHORUS_H_
#define CHORUS_H_
//
#include <stdio.h>
#include <ezdsp5535.h>
//
void chorusSetDelay(Int16 adcvalue);
void chorusSetRate(Int16 adcvalue);
void chorusSetMix(Int16 adcvalue);
Int16 chorus(Int16 x);

#endif /*CHORUS_H_*/
