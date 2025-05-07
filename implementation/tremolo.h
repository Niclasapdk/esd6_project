#ifndef TREMOLO_H_
#define TREMOLO_H_

#include <stdio.h>
#include <ezdsp5535.h>

Int16 tremolo(Int16 x);
Int16 tremoloChangeRate(Int16 dir);
Int16 tremoloChangeDepth(Int16 dir);

#endif /*TREMOLO_H_*/
