#include <ezdsp5535.h>
#include <ezdsp5535_i2s.h>
#include <ezdsp5535_i2c.h>
#include "codec.h"
#include "gpio.h"
#include "adc.h"
#include "lcd.h"
#include <stdio.h>

// FX
#include "wah.h"
//extern Int16 ts808(Int16);
//extern void setDriveResist(Int16);
#include "distortion.h"
#include "chorus.h"
#include "flanger.h"
#include "tremolo.h"
#include "reverb.h"

// Order of effects which gives their indices
typedef enum {
	FX_WAH,
	FX_OD,
	FX_CHORUS,
	FX_FLANGER,
	FX_TREMOLO,
	FX_REVERB,
	NUM_FX // Number of FX
} fxIdx_e;

// FX parameters (adjusted by pedal)
typedef enum {
	PARAM_WAH_PEDAL,
	PARAM_OD_DRIVE,
	PARAM_OD_LEVEL,
	PARAM_CHORUS_MIX,
	PARAM_CHORUS_RATE,
	PARAM_CHORUS_DELAY,
	PARAM_FLANGER_MIX,
	PARAM_FLANGER_RATE,
	PARAM_FLANGER_DELAY,
	PARAM_TREMOLO_DEPTH,
	PARAM_TREMOLO_RATE,
	PARAM_REVERB_MIX,
	PARAM_REVERB_TIME,
	NUM_FX_PARAMS // Number of FX parameters
} fxParamIdx_e;

// FX function pointers
// these take sample as input and returns result
static Int16 (*fx[NUM_FX])(Int16);
// FX parameter function pointers
// these take adc value from potentiometer and update parameters accordingly
static void (*fxParam[NUM_FX_PARAMS])(Int16);
// FX toggling
static Uint8 fxOn = 0;
// Current FX being modified by menu
static Uint8 menuCurrentFx = PARAM_FLANGER_MIX;

int main() {
	Int16 fuck, adcVal;
	Int16 i;
	Uint16 gpios;
	Int16 switchFxCtr = 0;

    EZDSP5535_init();
    EZDSP5535_I2C_init();
	initAdc();
	initCODEC();
	// Setup PIN MUX to in mode 2 for SD0 and SD1 (means all are GPIO)
	fuck = *EBSR;
	*EBSR = (fuck&0xf0ff)|0x0a00;
	*IODIR1 = 0;

	// FX function pointer setup
	fx[FX_WAH] = wah;
	fx[FX_OD] = tanhDistortion;
	fx[FX_FLANGER] = flanger_FIR;
	fx[FX_CHORUS] = chorus;
	fx[FX_TREMOLO] = tremolo;
	fx[FX_REVERB] = reverb;
	
	// FX param function pointer setup
	fxParam[PARAM_WAH_PEDAL] = setWahPedal;
	fxParam[PARAM_OD_DRIVE] = distSetDrive;
	fxParam[PARAM_OD_LEVEL] = distSetLevel;
	fxParam[PARAM_CHORUS_MIX] = chorusSetMix;
	fxParam[PARAM_CHORUS_RATE] = chorusSetRate;
	fxParam[PARAM_CHORUS_DELAY] = chorusSetDelay;
	fxParam[PARAM_FLANGER_MIX] = flangerSetMix;
	fxParam[PARAM_FLANGER_RATE] = flangerSetRate;
	fxParam[PARAM_FLANGER_DELAY] = flangerSetDelay;
	fxParam[PARAM_TREMOLO_DEPTH] = tremoloSetDepth;
	fxParam[PARAM_TREMOLO_RATE] = tremoloSetRate;
	fxParam[PARAM_REVERB_MIX] = reverbSetMix;
	fxParam[PARAM_REVERB_TIME] = reverbSetTime;
	
	// Initialize by setting all params with pedal half on
	for (i=0; i<NUM_FX_PARAMS; i++) fxParam[i](512);

	// Infinite loop
	while (1) {
		// Read switches, toggle fx, and update menu
		if (switchFxCtr++ == 1000) { // Update once per n samples
			switchFxCtr = 0;
			gpios = *IOINDATA1;
			fxOn = gpios>>10;
		}

		// Read potentiometer and change FX parameters
		adcVal = readAdcBlocking(3);
		fxParam[menuCurrentFx](adcVal);

		// Process sample
		EZDSP5535_I2S_readLeft(&fuck);
		for (i=0; i<NUM_FX; i++) {
			if (fxOn&(1<<i)) {
				fuck = fx[i](fuck);
			}
		}
		EZDSP5535_I2S_writeLeft(fuck);
	}
}
