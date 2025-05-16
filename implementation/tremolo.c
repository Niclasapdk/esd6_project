#include "tremolo.h"
//range (100-10000mHz)

static Int16 Depth = 16383; //range (3277 - 32439) Q15 (3277 = 0.1, 16383 = 0.5)Q14

/*Parameters for tremoloLFO*/
extern long EPM(long *, long *);

#define LFO_NORMALIZATION 23170 // [-1,1] normalize output
#define MAP_BY_TWO_PI_FS 1530 //2*pi*mapfactor/fs,fs=4410, mapfactor=0.1*2^15/100
#define THREE_FRAC_TWO 1610612736 // 1.5 Q2.30

static long oneMinusKpow2Frac2 = 2147429145;
static long k = 15300000; // the value is 0.0142 but changed to Q1.31

void tremoloFRate(Int16 r)
{
    k = ((long)MAP_BY_TWO_PI_FS * r) << 1; //Q1.31
    oneMinusKpow2Frac2 = (EPM(&k, &k) >> 1); //k^2/2
    oneMinusKpow2Frac2 = (2147483648 - oneMinusKpow2Frac2); //Q1.31
}

Int16 tremoloChangeRate(Int16 dir) {
    static Int16 rate = 5000;
    const Int16 step = 100;
    rate += dir*step;
    // saturate
    if (rate > 10000) rate = 10000;
    else if (rate < 1000) rate = 1000;
    tremoloFRate(rate);
    return rate;
}

Int16 tremoloChangeDepth(Int16 dir) {
    const Int16 step = 1638; // 5% step size
    Depth += dir*step;
    // saturate (order matters)
    if (Depth < -20000) Depth = 32767;
    else if (Depth < 0) Depth = 0;
    return Depth;
}

Int16 tremoloLFO() {
    Int16 out = 0;
    static long delaylineLFO[2] = {1518270939,0}; //Q1.31 format for 0.707 and 0
    long agc = 1073741824; // 1 for start gain
    Int16 outTemporary;
    long y1 = (EPM(&oneMinusKpow2Frac2, &delaylineLFO[0]) + EPM(&k, &delaylineLFO[1]));
    long y2 = (EPM(&oneMinusKpow2Frac2, &delaylineLFO[1]) - EPM(&k, &delaylineLFO[0]));
    long power = (EPM(&y1, &y1) + EPM(&y2, &y2)) >> 1; //Q2.30, so its possible to fit 1.5 in the
    agc = (THREE_FRAC_TWO - power); // Q1.31
    y1 = EPM(&y1, &agc) << 1;
    y2 = EPM(&y2, &agc) << 1;
    /*next iteration*/
    delaylineLFO[0] = y1;
    delaylineLFO[1] = y2;
    /*Output change to match -1 and 1*/
    outTemporary = _rnd(y1) >> 16;
    out = ((long)outTemporary * LFO_NORMALIZATION) >> 14;
    return out;
}

Int16 tremolo(Int16 x) {
    static Int16 lfoIndex = 10;
    static Int16 lfoValue = 0;
    Int16 TremoloFx = 0,y = 0;
    const Int16 initVal = 0; // 32767 = 1 Q0.15

    if(lfoIndex >= 9) {
        lfoValue = tremoloLFO(); //Change to lfo name.
        lfoIndex = 0;
    } else {
        lfoIndex++;
    }

    TremoloFx = (initVal+((Int32)Depth * lfoValue)>>15); // Q15 Tremolo value effect
    y = (((Int32)x * TremoloFx)>>15); // Q15 Modulated signal with tremolo effect
    return y;
}
