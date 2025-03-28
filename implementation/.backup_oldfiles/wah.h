#ifndef WAH_H_
#define WAH_H_

#include <ezdsp5535.h>
#include "lfo.h"

Int16 wah(Int16 x);
Int16 wahFx(Int16 x);
void FcSet(Int16 Fc, Int16 *F1);

#endif /*WAH_H_*/
