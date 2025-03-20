#include <stdio.h>

#include <ezdsp5535_i2s.h>
#include "codec.h"

// FX
#include "wah.h"

#define NUM_FX 6
Int16 (*fx[NUM_FX])(Int16);
Uint8 fxOn = 0;

int main() {
	Int16 fuck;
	int i;
    EZDSP5535_init( );
	printf("hello motherfucker");
	initCODEC();
	fx[0] = wah;
	fxOn |= 1;
	
	while (1) {
		EZDSP5535_I2S_readLeft(&fuck);
		for (i=0; i<NUM_FX; i++) {
			if (fxOn&(1<<i)) {
				fuck = fx[i](fuck);
			}
		}
		EZDSP5535_I2S_writeLeft(fuck);
	}
}
