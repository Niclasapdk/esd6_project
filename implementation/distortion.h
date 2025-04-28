#ifndef DISTORTION_H_
#define DISTORTION_H_

#include <ezdsp5535.h>

void distChangeDrive(Int16 dir);
void distChangeLevel(Int16 dir);
Int16 tanhDistortion(Int16 x);

#endif /*DISTORTION_H_*/
