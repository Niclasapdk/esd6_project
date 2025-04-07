#ifndef TREMOLO_H_
#define TREMOLO_H_

#include <stdio.h>
#include <ezdsp5535.h>

Int16 tremolo(Int16 x);
void tremoloSetRate(Int16 adcVal);
void tremoloSetMix(Int16 adcVal2);

#endif /*TREMOLO_H_*/