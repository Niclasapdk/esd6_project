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
// these take direction value (1 or -1) and update parameters accordingly
static void (*fxParam[NUM_FX_PARAMS])(Int16);
// FX toggling
static Uint8 fxOn = 0;
// Current FX being modified by menu
static Uint8 menuCurrentFx = PARAM_CHORUS_MIX;

int main() {
	Int16 fuck, adcVal;
	Int16 i;
	Uint16 gpios, menuBtns;
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
	fxParam[PARAM_OD_DRIVE] = distChangeDrive;
	fxParam[PARAM_OD_LEVEL] = distChangeLevel;
	fxParam[PARAM_CHORUS_MIX] = chorusChangeMix;
	fxParam[PARAM_CHORUS_RATE] = chorusChangeRate;
	fxParam[PARAM_CHORUS_DELAY] = chorusChangeDelay;
	fxParam[PARAM_FLANGER_MIX] = flangerChangeMix;
	fxParam[PARAM_FLANGER_RATE] = flangerChangeRate;
	fxParam[PARAM_FLANGER_DELAY] = flangerChangeDelay;
	fxParam[PARAM_TREMOLO_DEPTH] = tremoloChangeDepth;
	fxParam[PARAM_TREMOLO_RATE] = tremoloChangeRate;
	fxParam[PARAM_REVERB_MIX] = reverbChangeMix;
	fxParam[PARAM_REVERB_TIME] = reverbChangeTime;

	// Infinite loop
	while (1) {
		// Read switches, toggle fx, and update menu
		if (switchFxCtr++ == 1000) { // Update once per n samples
			switchFxCtr = 0;
			gpios = *IOINDATA1;
			// toggle FX
			fxOn = gpios>>10;
			// change selected parameter
			menuBtns = gpios&0x3; // seperate menu buttons from rest of gpios
			if (menuBtns == 0x1) { // change currently selected param up
				// increment and check for out-of-bounds index
				if (++menuCurrentFx == NUM_FX_PARAMS) menuCurrentFx = 0;
			} else if (menuBtns == 0x2) { // change currently selected param down
				// decrement and check for overflow
				// if overflow occurs, menuCurrentFx will be maxInt so larger than NUM_FX_PARAMS
				if (--menuCurrentFx > NUM_FX_PARAMS) menuCurrentFx = NUM_FX_PARAMS-1;
			}
			// change parameter
			menuBtns = gpios&0xc; // seperate param change buttons from rest of gpios
			if (menuBtns == 0x4) { // change fx param up
				fxParam[menuCurrentFx](1);
			} else if (menuBtns == 0x8) { // change fx param down
				fxParam[menuCurrentFx](-1);
			}
		}

		// Read potentiometer and change wah pedal
		adcVal = readAdcBlocking(3);
		setWahPedal(adcVal);

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
