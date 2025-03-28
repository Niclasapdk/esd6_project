#ifndef Chorus_H_
#define Chorus_H_

#include <ezdsp5535.h>
#include "lfo.h"

Int16 ChorusFx(Int16 x);
void getRandomDelay(Int16 DelayMin, Int16 DelayMax,Int16 Voices,ushort Delays);
void depthChorusSet(Int16 Depth);
void rateChorusSet(Int16 Rate);
void delayChorusSet(Int16 Delay);

#endif /*Chorus_H_*/
