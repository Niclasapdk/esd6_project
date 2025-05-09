#include <EZDSP5535.h>

#include "od_LUT.h"

#define OD_AA_FILTER_TAP_NUM 57
static Int16 odAaFilterTaps[OD_AA_FILTER_TAP_NUM] = {
    31,    32,   -44,  -175,  -224,   -85,   129,   160,
   -66,  -267,  -119,   254,   343,   -75,  -499,  -272,
   447,   683,   -83,  -965,  -607,   865,  1514,   -87,
 -2431, -1937,  3113,  9874, 13019,  9874,  3113, -1937,
 -2431,   -87,  1514,   865,  -607,  -965,   -83,   683,
   447,  -272,  -499,   -75,   343,   254,  -119,  -267,
   -66,   160,   129,   -85,  -224,  -175,   -44,    32,
    31
};
static Int16 odAaFilterDline[OD_AA_FILTER_TAP_NUM+2] = {0};
extern Uint16 fir(Int16 *x, Int16 *h, Int16 *r, Int16 *dbuffer, Uint16 nx, Uint16 nh);

// piecewise constants in Q15
#define TH1   10922        //  1/3 * 32768
#define TH2   21845        //  2/3 * 32768
#define C2    (2 << 15)    //  2.0 in Q15
#define C3    (3 << 15)    //  3.0 in Q15
#define NUM3DIV4 24576 // 3/4 in Q15
#define INV12 2733 // 1/12 in Q15

// Equaliser filter coefficients
static Int16 hp_b[3] = {16057, -32114, 16057};
static Int16 hp_a[3] = {16384, -32108, 15737};
static Int16 lp_b[3] = {2904, 5808, 2904};
static Int16 lp_a[3] = {16384, -8335, 3567};

static Int16 drive = 10;        // Q0 integer 1..50
static Int16 level = 16384;     // Q15 level
// noise gate
static Int16 gateThres = 0; // threshold for noise gate
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
	Int16 y;
	fir(&x, odAaFilterTaps, &y, odAaFilterDline, 1, OD_AA_FILTER_TAP_NUM);
    return y;
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
    Int16 sign, u, v, usquare, imm;
    sign = (x >= 0) ? 1 : -1;
    // avoid overflow when -32768
    if (x==-32768) u = 32767;
    else u = sign*x;

    if (u < TH1) {
        // v = 2 * y3
        v   = x<<1; // will not overflow since u < TH1
    }
    else if (u < TH2) {
        // v = sign * [ (3 - (2 - 3*u)^2) / 3 ]
        // v = sign * 4*[-1/12 - 3/4*x^2 + x]
        usquare = ((Int32)u*(Int32)u)>>15; // u^2
        imm = ((Int32)usquare*NUM3DIV4)>>15; // 3/4*x^2
        v = (-INV12-imm)+u;
        v = 4 * v;
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
//    y2 = odFirAA(y1);
	// FIR filter omitted because real-time feasible FIR filters
	// did not make significant improvement to sound
	y2 = y1;

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
