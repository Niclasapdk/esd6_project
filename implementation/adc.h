#ifndef ADC_H_
#define ADC_H_

#include <ezdsp5535.h>

void initAdc();
Int16 readAdcBlocking(Uint8 ch);

#endif /*ADC_H_*/
