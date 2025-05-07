#ifndef REVERB_H_
#define REVERB_H_


#include <EZDSP5535.h>

Int16 reverb(Int16 x);
Int16 reverbChangeTime(Int16 dir);
Int16 reverbChangeMix(Int16 dir);

#endif /*REVERB_H_*/
