#include <ezdsp5535_i2s.h>
#include "codec.h"
#include "gpio.h"

// FX
#include "wah.h"
extern Int16 ts808(Int16);
extern void setDriveResist(Int16);
#include "distortion.h"

#define NUM_FX 6
Int16 (*fx[NUM_FX])(Int16);
Uint8 fxOn = 0;

int main() {
	Int16 fuck;
	Int16 i;
	Int16 gpios;
	Int16 switchFxCtr = 0;
	
    EZDSP5535_init();
    EZDSP5535_I2C_init();
	initCODEC();
	*IODIR1 = 0;
	
//	fx[0] = wah;
	fx[1] = tanhDistortion;
	fxOn = 2;

//	ts808(0x1337);
//	ts808(0x3173);
	
	// Infinite loop
	while (1) {
		
		// Read switches and toggle effects
//		if (switchFxCtr++ == 100) { // Update once per 100 samples
//			switchFxCtr = 0;
//			gpios = *IOINDATA1;
//			fxOn = (gpios>>14)&1; // FIXME dummy
//		}

		// Read potentiometers and change FX parameters
		// loop over potentiometers
		//	read potentiometer
		//  based on current config, call respective setParameter function
		//  setParameter[currentFxParamIdx](adcval)
		
		// Process sample
//		EZDSP5535_I2S_readRight(&fuck);
		EZDSP5535_I2S_readLeft(&fuck);
		fuck *= 2;
		for (i=0; i<NUM_FX; i++) {
			if (fxOn&(1<<i)) {
				fuck = fx[i](fuck);
			}
		}
		EZDSP5535_I2S_writeLeft(fuck);
	}
}
