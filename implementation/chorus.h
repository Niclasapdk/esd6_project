#ifndef CHORUS_H_
#define CHORUS_H_
//
#include <stdio.h>
#include <ezdsp5535.h>
//
void chorusSetDelay(Int16 adcvalue);
void chorusSetRate(Int16 adcvalue);
void chorusSetDepth(Int16 adcvalue);
Int16 chorus(Int16 x);
//void chorusFRate(Int16 r);
//
#endif /*CHORUS_H_*/