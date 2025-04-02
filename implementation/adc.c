#include <ezdsp5535.h>
#include "adc.h"

ioport int *SARCTRL    = (ioport int *)0x7012; // SAR A/D Control Register Section 3.1
ioport int *SARDATA    = (ioport int *)0x7014; // SAR A/D Data Register Section 3.2
ioport int *SARCLKCTRL = (ioport int *)0x7016; // SAR A/D Clock Control Register Section 3.3
ioport int *SARPINCTRL = (ioport int *)0x7018; // SAR A/D Reference and Pin Control Register Section 3.4
ioport int *SARGPOCTRL = (ioport int *)0x701A; // SAR A/D GPO Control Register Section 3.5


void initAdc() {
	*SARCTRL = 0x4000;    // single conversion mode
	*SARCLKCTRL = 0;      // fastest conversion mode
	*SARPINCTRL = 0x3100; // (PWRUPBIAS|SARPWRUP|REFAVDDSEL)
	*SARGPOCTRL = 0;      // all analog inputs as analog inputs (not gpio)
}

// Performs a blocking adc conversion
// ch: channel and must be from 0 to 3
Int16 readAdcBlocking(Uint8 ch) {
	*SARCTRL |= 0x8000; // set ADCSTRT to start conversion
	while ((*SARDATA) & 0x8000); // wait until ADCBUSY is cleared
	return (*SARDATA) & 0x03ff; // return 10-bit result
}
