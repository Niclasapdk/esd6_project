#ifndef GPIO_H_
#define GPIO_H_

#include <ezdsp5535.h>

ioport int *IODIR1     = (ioport int *)0x1C06;
ioport int *IOINDATA1  = (ioport int *)0x1C08;
ioport int *IOOUTDATA1 = (ioport int *)0x1C0A;
ioport int *EBSR       = (ioport int *)0x1C00;

void initGpios();

#endif /*GPIO_H_*/
