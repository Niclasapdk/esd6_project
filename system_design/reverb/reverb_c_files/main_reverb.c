#include <ezdsp5535_i2s.h>
#include "codec_reverb.h"
#include <ezdsp5535_i2c.h>
#include "reverb.h"

int main() {
	Int16 fuck = 0;
	Int16 reverb_mix = 6554;//Q15 val max 32767  0.1=3270 0.2=6554 0.3=10000 0.55=18000
	Int16 reverb_time = 500;//adc val max 1023
	
    EZDSP5535_init();
    EZDSP5535_I2C_init();
	initCODEC();

	// Infinite loop
	while (1) {
		// Process sample
		EZDSP5535_I2S_readLeft(&fuck);
		setReverbMix(reverb_mix);
		setReverbTime(reverb_time);
		fuck = reverb(fuck);
		EZDSP5535_I2S_writeRight(fuck);
	}
}
