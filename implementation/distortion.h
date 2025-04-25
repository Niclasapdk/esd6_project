#ifndef DISTORTION_H_
#define DISTORTION_H_

#include <ezdsp5535.h>

void distSetDrive(Int16 adcVal);
void distSetLevel(Int16 adcVal);
Int16 tanhDistortion(Int16 x);

#endif /*DISTORTION_H_*/
