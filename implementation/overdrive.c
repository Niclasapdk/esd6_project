#include <EZDSP5535.h>

#include "od_LUT.h"

// piecewise constants in Q15
#define TH1   10923        //  1/3 * 32768
#define TH2   21845        //  2/3 * 32768
#define C2    (2 << 15)    //  2.0 in Q15
#define C3    (3 << 15)    //  3.0 in Q15
#define INV3  10923        //  1/3 in Q15
#define NUM2DIV3 21845 // 2/3 in Q15

// Equaliser filter coefficients
static Int16 hp_b[3] = {16057, -32114, 16057};
static Int16 hp_a[3] = {16384, -32108, 15737};
static Int16 lp_b[3] = {2904, 5808, 2904};
static Int16 lp_a[3] = {16384, -8335, 3567};

static Int16 drive = 10;        // Q0 integer 1..50
static Int16 level = 16384;     // Q15 level
// noise gate
static Int16 gateThres = 200; // threshold for noise gate
static Uint16 gateRel = 441; // release time (samples) before noise gate activates

Int16 odChangeGateThres(Int16 dir) {
    const Int16 step = 50;
    gateThres += dir*step;
    if (gateThres > 400) gateThres = 400;
    else if (gateThres < 0) gateThres = 0;
    return gateThres;
}

Int16 odChangeGateRel(Int16 dir) {
    const Int16 step = 4410;
    gateRel += dir*step;
    if (gateRel > 50000) gateRel = 1;
    else if (gateRel > 44100) gateRel = 44100;
    return gateRel/441;
}

Int16 odChangeDrive(Int16 dir) {
    const Int16 step = 5;
    drive += dir*step;
    if (drive > 50) drive = 50;
    else if (drive < 1) drive = 1;
    return drive;
}

Int16 odChangeLevel(Int16 dir) {
    const Int16 step = 2000;
    level += dir*step;
    // saturate (order matters)
    if (level < -20000) level = 32767;
    else if (level < 0) level = 0;
    return level;
}

Int16 odChangeHpCutoff(Int16 dir) {
    static Int16 idx = 1;
    idx += dir;
    if (idx < 0) idx = 0;
    else if (idx >= OD_HP_LUT_LEN) idx = OD_HP_LUT_LEN-1;
    hp_b[0] = hp_b_table[idx][0];
    hp_b[1] = hp_b_table[idx][1];
    hp_b[2] = hp_b_table[idx][2];
    hp_a[0] = hp_a_table[idx][0];
    hp_a[1] = hp_a_table[idx][1];
    hp_a[2] = hp_a_table[idx][2];
    return hp_fc_list[idx];
}

Int16 odChangeLpCutoff(Int16 dir) {
    static Int16 idx = 4;
    idx += dir;
    if (idx < 0) idx = 0;
    else if (idx >= OD_LP_LUT_LEN) idx = OD_LP_LUT_LEN-1;
    lp_b[0] = lp_b_table[idx][0];
    lp_b[1] = lp_b_table[idx][1];
    lp_b[2] = lp_b_table[idx][2];
    lp_a[0] = lp_a_table[idx][0];
    lp_a[1] = lp_a_table[idx][1];
    lp_a[2] = lp_a_table[idx][2];
    return lp_fc_list[idx];
}

static inline Int16 sat_q15(Int32 x)
{
    if (x >  0x7FFF) return  0x7FFF;
    // compiler sees -0x8000 as 0x8000 because of the way it interprets numeric literals
    // this is the reason for -32768 in the comparison
    if (x < -32768) return -0x8000;
    return (Int16)x;
}

Int16 odNoiseGate(Int16 x) {
    static Uint16 cnt = 0;
    if (x < gateThres && x > -gateThres) {
        // signal smaller than threshold
        if (cnt == gateRel) {
            return 0;
        } else {
            cnt++;
        }
    } else {
        cnt = 0;
    }
    return x;
}

// Anti-aliasing FIR filter
Int16 odFirAA(Int16 x) {
    return x; // TODO implement fir
}

// 2nd order highpass IIR filter (direct form 1)
Int16 odIirHp(Int16 x) {
    Int16 y;
    static Int16 hp_xz[2] = {0,0};
    static Int16 hp_yz[2] = {0,0};
    static Int32 acc;
    acc  = (Int32)hp_b[0] * x;
    acc += (Int32)hp_b[1] * hp_xz[0];
    acc += (Int32)hp_b[2] * hp_xz[1];
    acc -= (Int32)hp_a[1] * hp_yz[0];
    acc -= (Int32)hp_a[2] * hp_yz[1];
    y = sat_q15(acc >> 14);
    hp_xz[1] = hp_xz[0];
    hp_xz[0] = x;
    hp_yz[1] = hp_yz[0];
    hp_yz[0] = y;
    return y;
}

// 2nd order lowpass IIR filter (direct form 1)
Int16 odIirLp(Int16 x) {
    Int16 y;
    static Int16 lp_xz[2] = {0,0};
    static Int16 lp_yz[2] = {0,0};
    static Int32 acc;
    acc  = (Int32)lp_b[0] * x;
    acc += (Int32)lp_b[1] * lp_xz[0];
    acc += (Int32)lp_b[2] * lp_xz[1];
    acc -= (Int32)lp_a[1] * lp_yz[0];
    acc -= (Int32)lp_a[2] * lp_yz[1];
    y = sat_q15(acc >> 14);
    lp_xz[1] = lp_xz[0];
    lp_xz[0] = x;
    lp_yz[1] = lp_yz[0];
    lp_yz[0] = y;
    return y;
}

// Static nonlinear piecewise polygon
Int16 odNonlinPoly(Int16 x) {
    Int16 sign, u, v, imm;
    sign = (x >= 0) ? 1 : -1;
    u    = sign*x;

    if (u < TH1) {
        // v = 2 * y3
        v   = x<<1; // will not overflow since u < TH1
    }
    else if (u < TH2) {
        // v = sign * [ (3 - (2 - 3*u)^2) / 3 ]
        // v = sign * [1 - (2/3 - u)^2]
        imm = (NUM2DIV3 - u);
        v = ((Int32)imm*(Int32)imm)>>15;
        v = 0x7fff-v;
        v = sign * v;
    }
    else {
        // hard-limit to +/-1.0
        v = (sign > 0) ?  0x7FFF : -32768;
    }
    return v;
}

Int16 overdrive(Int16 x) {
    Int32 acc;
    Int16 y1, y2, y3, y4, y5;

    // Noise gate
    y1 = odNoiseGate(x);

    // FIR anti-alias LP
    y2 = odFirAA(y1);

    // IIR high-pass (Direct Form I)
    y3 = odIirHp(y2);

    // Drive-gain (Q0 * Q15 -> Q15 saturating)
    acc = (Int32)drive * y3;
    y3  = sat_q15(acc);

    // Piecewise soft-clip on y2 (Q15)
    y4 = odNonlinPoly(y3);

    // 4) IIR low-pass
    y5 = odIirLp(y4);

    // 5) Level-scale (Q15 * Q15 -> Q30 >>15 -> Q15)
    acc = (Int32)level * y5;
    acc >>= 15;
    return sat_q15(acc);
}
