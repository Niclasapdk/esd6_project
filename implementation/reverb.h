#ifndef REVERB_H_
#define REVERB_H_


#include <EZDSP5535.h>

Int16 reverb(Int16 x);
void reverbChangeTime(Int16 dir);
void reverbChangeMix(Int16 dir);

#endif /*REVERB_H_*/
