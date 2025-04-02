#include <ezdsp5535.h>
#include <ezdsp5535_i2s.h>
#include <ezdsp5535_i2c.h>
#include "codec.h"
#include "gpio.h"
#include "adc.h"

// FX
#include "wah.h"
extern Int16 ts808(Int16);
extern void setDriveResist(Int16);
#include "distortion.h"

// dummy function for the setParam function pointers
void dummyFunc(Int16 adcVal) {}

// Order of effects which gives their indices
typedef enum {
	FX_WAH,
	FX_OD,
	FX_CHORUS,
	FX_FLANGER,
	FX_TREMOLO,
	FX_REVERB
} fxIdx_e;

// Number of FX
#define NUM_FX 6
// FX function pointers
static Int16 (*fx[NUM_FX])(Int16);
// FX toggling
static Uint8 fxOn = 0;
// Current FX being modified by menu
static Uint8 menuCurrentFx = 0;
// FX parameter function pointers
// these take adc value from potentiometer and update parameters accordingly
static void (*fxParam1[NUM_FX])(Int16);
static void (*fxParam2[NUM_FX])(Int16);
static void (*fxParam3[NUM_FX])(Int16);

int main() {
	Int16 fuck;
	Int16 i;
	Int16 gpios;
	Int16 switchFxCtr = 0;
	
    EZDSP5535_init();
    EZDSP5535_I2C_init();
	initAdc();
	initCODEC();
	*IODIR1 = 0;
	
	//// FX setup
	// WAH
	fx[FX_WAH] = dummyFunc; //FIXME should be wah when it is made
	fxParam1[FX_WAH] = setWahPedal;
	fxParam2[FX_WAH] = dummyFunc;
	fxParam3[FX_WAH] = dummyFunc;
	// Overdrive
	fx[FX_OD] = tanhDistortion;
	fxParam1[FX_OD] = distSetDrive;
	fxParam2[FX_OD] = dummyFunc;
	fxParam3[FX_OD] = dummyFunc;
	
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
		fuck = readAdcBlocking(0);
		fxParam1[menuCurrentFx](fuck);
		fuck = readAdcBlocking(1);
		fxParam2[menuCurrentFx](fuck);
		fuck = readAdcBlocking(2);
		fxParam3[menuCurrentFx](fuck);
		
		// Process sample
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
