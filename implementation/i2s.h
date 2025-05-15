#ifndef I2S_H_
#define I2S_H_

#include <ezdsp5535.h>

void initI2S();
Int16 i2sReadL();
Int16 i2sReadR();
void i2sWriteL(Int16 x);
void i2sWriteR(Int16 x);

#endif /*I2S_H_*/
