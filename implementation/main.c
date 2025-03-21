#include <ezdsp5535_i2s.h>
#include "codec.h"
#include "gpio.h"

// FX
#include "wah.h"
extern Int16 ts808(Int16);

#define NUM_FX 6
Int16 (*fx[NUM_FX])(Int16);
Uint8 fxOn = 0;

int main() {
	Int16 fuck;
	Int16 i;
	Int16 gpios;
	Int16 switchFxCtr = 0;
    EZDSP5535_init();
	initCODEC();
	*IODIR1 = 0;
	
	fx[0] = wah;
	fx[1] = ts808;
	fxOn |= 1;
	
	// Infinite loop
	while (1) {
		
		// Read switches and toggle effects
		if (switchFxCtr++ == 100) { // Update once per 100 samples
			switchFxCtr = 0;
			gpios = *IOINDATA1;
			fxOn = (gpios>>14)&1; // FIXME dummy
		}
		EZDSP5535_I2S_readLeft(&fuck);
		for (i=0; i<NUM_FX; i++) {
			if (fxOn&(1<<i)) {
				fuck = fx[i](fuck);
			}
		}
		EZDSP5535_I2S_writeLeft(fuck);
	}
}
