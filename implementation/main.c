#include <ezdsp5535.h>
#include <ezdsp5535_i2s.h>
#include <ezdsp5535_i2c.h>
#include <ezdsp5535_uart.h>
#include "codec.h"
#include "gpio.h"
#include "adc.h"
#include "lcd.h"
#include <stdio.h>
#include "i2s.h"

// FX
#include "wah.h"
#include "overdrive.h"
#include "chorus.h"
#include "flanger.h"
#include "tremolo.h"
#include "reverb.h"

//#define MEASURE_COMPUTATION_TIME

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
    PARAM_OD_HP,
    PARAM_OD_LP,
    PARAM_OD_GATE_THRES,
    PARAM_OD_GATE_REL,
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
static Int16 (*fxParam[NUM_FX_PARAMS])(Int16);
// FX toggling
static Uint8 fxOn = 0;
// Current FX being modified by menu
static Uint8 menuCurrentFx = PARAM_OD_DRIVE;

// Main loop routines
Uint16 toggleFx();
Int16 processFx(Int16 x);
void processUi();
void sendMenuStatus(Uint8 idx, Int16 val);
void setupClock();
void bypassClock();
int main() {
    Int16 fuck;
    bypassClock();
    EZDSP5535_init();
    EZDSP5535_I2C_init();
    initCODEC();
    EZDSP5535_UART_open(); // must be before initI2S
    initI2S();
    initAdc();
    setupClock();
    // Setup PIN MUX to in mode 2 for SD0 and SD1 (means all are GPIO)
    fuck = *EBSR;
    *EBSR = (fuck&0xf0ff)|0x0a00;
    *IODIR1 = 0x0030; // GPIO 4 and 5 as output, rest as input
    *IOOUTDATA1 = 0;

    // FX function pointer setup
    fx[FX_WAH] = wah;
    fx[FX_OD] = overdrive;
    fx[FX_FLANGER] = flanger_FIR;
    fx[FX_CHORUS] = chorus;
    fx[FX_TREMOLO] = tremolo;
    fx[FX_REVERB] = reverb;

    // FX param function pointer setup
    fxParam[PARAM_OD_DRIVE] = odChangeDrive;
    fxParam[PARAM_OD_LEVEL] = odChangeLevel;
    fxParam[PARAM_OD_HP] = odChangeHpCutoff;
    fxParam[PARAM_OD_LP] = odChangeLpCutoff;
    fxParam[PARAM_OD_GATE_THRES] = odChangeGateThres;
    fxParam[PARAM_OD_GATE_REL] = odChangeGateRel;
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

    odInit();
    flangerInit();
    chorusInit();
    reverbInit();
    sendMenuStatus(menuCurrentFx, 10);

    // Infinite loop
    while (1) {
#ifdef MEASURE_COMPUTATION_TIME
        *IOOUTDATA1 = 0x00;
#endif
        fuck = i2sReadR();
#ifdef MEASURE_COMPUTATION_TIME
        *IOOUTDATA1 = 0x20;
#endif
        // Read switches and toggle fx
        if (toggleFx()) {
            // bypass is on (handle ui)
            processUi();
        } else {
            // bypass is off (process audio)
#ifdef MEASURE_COMPUTATION_TIME
            *IOOUTDATA1 |= 0x10;
#endif
            fuck = processFx(fuck);
#ifdef MEASURE_COMPUTATION_TIME
            *IOOUTDATA1 &= ~0x10;
#endif
            i2sWriteR(fuck);
        }
    }
}

void setupClock() {
    // Steps from section 1.4.3.2.6 Software Steps To Modify Multiplier and Divider Ratios
    // from TMS320C5505 DSP System User's Guide
    // since C5535 and C5505 have same PLL
    ioport int *CCR2  = (ioport int *)0x1C1F;
    ioport int *CGCR1 = (ioport int *)0x1C20;
    ioport int *CGCR2 = (ioport int *)0x1C21;
    ioport int *CGCR3 = (ioport int *)0x1C22;
    ioport int *CGCR4 = (ioport int *)0x1C23;
    *CCR2 &= ~1; // bypass mode (SYSCLKSEL=0)
    *CGCR1 &= ~0x8000; // set RSVD=0
    *CGCR1 = 1000-4; // set M
    *CGCR2 = 120-4; // set RDRATIO
    *CGCR4 = 0; // SET OUTDIVEN=0
    *CGCR3 = 0x0806;
    *CGCR1 |= 0x8000; // set RSVD=1
    EZDSP5535_waitusec(5000); // wait minimum 4 ms
    *CCR2 |= 1; // bypass mode (SYSCLKSEL=0)
}

void bypassClock() {
    // Bypass PLL
    ioport int *CCR2  = (ioport int *)0x1C1F;
    *CCR2 &= ~1; // bypass mode (SYSCLKSEL=0)
}

// toggles fxOn, returns true if bypass is on else false
Uint16 toggleFx() {
    Uint16 gpios;
    gpios = *IOINDATA1;
    // fx stored in gp[10:15]
    fxOn = gpios>>10;
    // Return bypass status (to save an indata read)
    return gpios&0x80; // gp[7] is bypass
}

Int16 processFx(Int16 x) {
    Int16 i;
    // Process sample
#pragma UNROLL(NUM_FX)
    for (i=0; i<NUM_FX; i++) {
        if (fxOn&(1<<i)) {
            x = fx[i](x);
        }
    }
    return x;
}

void sendMenuStatus(Uint8 idx, Int16 val) {
    char buf[5];
    Int16 i;
    buf[0] = 0xaa; // start byte
    buf[1] = idx;
    buf[2] = ((Uint16)val)>>8;
    buf[3] = ((Uint8)val);
    buf[4] = 0xbb;
    for (i=0; i<5; i++) {
        EVM5515_UART_putChar(buf[i]);
    }
}

void processUi() {
#define UI_DEBOUNCE_LIMIT 8820
    static Uint16 debounce = UI_DEBOUNCE_LIMIT;
    Uint16 gpios, menuBtns;
    static Int16 val=0xa;
    if (debounce == UI_DEBOUNCE_LIMIT) {
        gpios = *IOINDATA1;
        // change selected parameter
        menuBtns = gpios&0x3; // seperate menu buttons from rest of gpios
        if (menuBtns == 0x1) { // change currently selected param up
            // increment and check for out-of-bounds index
            if (++menuCurrentFx == NUM_FX_PARAMS) menuCurrentFx = 0;
            debounce = 0;
            val = fxParam[menuCurrentFx](0); // read val for this param
        } else if (menuBtns == 0x2) { // change currently selected param down
            // decrement and check for overflow
            // if overflow occurs, menuCurrentFx will be maxInt so larger than NUM_FX_PARAMS
            if (--menuCurrentFx > NUM_FX_PARAMS) menuCurrentFx = NUM_FX_PARAMS-1;
            debounce = 0;
            val = fxParam[menuCurrentFx](0); // read val for this param
        }
        // change parameter
        menuBtns = gpios&0xc; // seperate param change buttons from rest of gpios
        if (menuBtns == 0x8) { // change fx param up
            val = fxParam[menuCurrentFx](1);
            debounce = 0;
        } else if (menuBtns == 0x4) { // change fx param down
            val = fxParam[menuCurrentFx](-1);
            debounce = 0;
        }
        // send menu status if a change was detected
        if (debounce == 0) sendMenuStatus(menuCurrentFx, val);
//        printf("Param: %x, Value: %x\n", menuCurrentFx, val);
    } else {
        debounce++;
    }
}
