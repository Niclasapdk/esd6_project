#include <ezdsp5535.h>
#include "i2s.h"

ioport int *I2SSCTRL = (ioport int *)0x2A00;
ioport int *I2SINTMASK = (ioport int *)0x2A14;
ioport int *I2SINTFL = (ioport int *)0x2A10;
ioport int *I2SRXLT1 = (ioport int *)0x2A29;
ioport int *I2SRXRT1 = (ioport int *)0x2A2D;
ioport int *I2STXLT1 = (ioport int *)0x2A09;
ioport int *I2STXRT1 = (ioport int *)0x2A0D;

void initI2S() {
	*I2SSCTRL = 0x8010; // enable | data width
	*I2SINTMASK = 0x002b; // XMITST | RCVST | FERR | OUERR
}

Int16 i2sReadL() {
	while (!(0x0008 & (*I2SINTFL))); // wait for data
	return *I2SRXLT1;
}

Int16 i2sReadR() {
	while (!(0x0008 & (*I2SINTFL))); // wait for data
	return *I2SRXRT1;
}

void i2sWriteL(Int16 x) {
	while (!(0x0020 & (*I2SINTFL))); // wait for ready
	*I2STXLT1 = x;
}

void i2sWriteR(Int16 x) {
	while (!(0x0020 & (*I2SINTFL))); // wait for ready
	*I2STXRT1 = x;
}
