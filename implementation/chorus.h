#ifndef CHORUS_H_
#define CHORUS_H_
//
#include <stdio.h>
#include <ezdsp5535.h>
//
void chorusChangeDelay(Int16 dir);
void chorusChangeRate(Int16 dir);
void chorusChangeMix(Int16 dir);
Int16 chorus(Int16 x);

#endif /*CHORUS_H_*/
