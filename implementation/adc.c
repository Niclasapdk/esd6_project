#include <ezdsp5535.h>
#include "adc.h"

ioport int *SARCTRL    = (ioport int *)0x7012; // SAR A/D Control Register Section 3.1
ioport int *SARDATA    = (ioport int *)0x7014; // SAR A/D Data Register Section 3.2
ioport int *SARCLKCTRL = (ioport int *)0x7016; // SAR A/D Clock Control Register Section 3.3
ioport int *SARPINCTRL = (ioport int *)0x7018; // SAR A/D Reference and Pin Control Register Section 3.4
ioport int *SARGPOCTRL = (ioport int *)0x701A; // SAR A/D GPO Control Register Section 3.5

void initAdc() {
	*SARGPOCTRL = 0xffff; // set all as output to try to fix this shit adc
	*SARGPOCTRL = 0;      // all analog inputs as analog inputs (not gpio)
	*SARPINCTRL = 0x061b; // set all weird values to reset them later to fix this shit adc
	*SARPINCTRL = 0x3601; // (PWRUPBIAS|SARPWRUP|FUCK)
	*SARCLKCTRL = 64;     // clock division (+1)
}

// Performs a blocking adc conversion
// ch: channel and must be from 0 to 3
Int16 readAdcBlocking(Uint16 ch) {
	Int16 result;
	// Setup SARCTRL register with channel, single conversion mode and start conversion
	// set ADCSTRT to start conversion
	*SARCTRL = 0x8000|(ch<<12)|(0x0400);
	// wait until ADCBUSY is set
	while (!((*SARDATA)& 0x8000));
//	while (1);
	// wait until ADCBUSY is cleared and channel matches requested channel
	while (result = (*SARDATA), (result & 0x8000) || ((result>>12)&0x7)!=ch);
	return result & 0x03ff; // return 10-bit result
}
