#ifndef ADC_H_
#define ADC_H_

#include <ezdsp5535.h>

void initAdc();
Int16 readAdcBlocking(Uint16 ch);
void adcStartConv();
Int16 adcReadDataNowait();

#endif /*ADC_H_*/
