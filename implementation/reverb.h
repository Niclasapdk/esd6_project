#ifndef REVERB_H_
#define REVERB_H_


#include <EZDSP5535.h>

Int16 reverb(Int16 x);
void reverbSetTime(Int16 adcVal);
void reverbSetMix(Int16 adcVal);

#endif /*REVERB_H_*/
