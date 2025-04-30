#include <EZDSP5535.h>

#include "od_LUT.h"

// piecewise constants in Q15
#define GATE_TH 200 // gate threshold
#define TH1   10923        //  1/3 * 32768
#define TH2   21845        //  2/3 * 32768
#define C2    (2 << 15)    //  2.0 in Q15
#define C3    (3 << 15)    //  3.0 in Q15
#define INV3  10923        //  1/3 in Q15

static Int16 hp_b[3] = {16057, -32114, 16057};
static Int16 hp_a[3] = {16384, -32108, 15737};
static Int16 lp_b[3] = {2904, 5808, 2904};
static Int16 lp_a[3] = {16384, -8335, 3567};
// delay lines
static Int16 hp_xz[2] = {0,0};
static Int16 hp_yz[2] = {0,0};
static Int16 lp_xz[2] = {0,0};
static Int16 lp_yz[2] = {0,0};

static Int16 drive = 10;        // Q0 integer 1..50
static Int16 level = 16384;     // Q15 level

void odChangeDrive(Int16 dir) {
	const Int16 step = 5;
	drive += dir*step;
	if (drive > 50) drive = 50;
	else if (drive < 1) drive = 1;
}

void odChangeLevel(Int16 dir) {
	const Int16 step = 2000;
	level += dir*step;
	// saturate (order matters)
	if (level < -20000) level = 32767;
	else if (level < 0) level = 0;
}

void odChangeHpCutoff(Int16 dir) {
	static Int16 idx = 1;
	idx += dir;
	if (idx < 0) idx = OD_HP_LUT_LEN-1;
	else if (idx >= OD_HP_LUT_LEN) idx = 0;
	hp_b[0] = hp_b_table[idx][0];
	hp_b[1] = hp_b_table[idx][1];
	hp_b[2] = hp_b_table[idx][2];
	hp_a[0] = hp_a_table[idx][0];
	hp_a[1] = hp_a_table[idx][1];
	hp_a[2] = hp_a_table[idx][2];
}

void odChangeLpCutoff(Int16 dir) {
	static Int16 idx = 4;
	idx += dir;
	if (idx < 0) idx = OD_LP_LUT_LEN-1;
	else if (idx >= OD_LP_LUT_LEN) idx = 0;
	lp_b[0] = lp_b_table[idx][0];
	lp_b[1] = lp_b_table[idx][1];
	lp_b[2] = lp_b_table[idx][2];
	lp_a[0] = lp_a_table[idx][0];
	lp_a[1] = lp_a_table[idx][1];
	lp_a[2] = lp_a_table[idx][2];
}

static inline Int16 sat_q15(Int32 x)
{
    if (x >  0x7FFF) return  0x7FFF;
    if (x < -32768) return -0x8000;
    return (Int16)x;
}

Int16 overdrive(Int16 x_in) {
    Int32 acc;
    Int16 y1, y2, y3, y4;
    Int16 sign, u, v;
    
    // Noise gate
    if (x_in < GATE_TH && x_in > -GATE_TH) x_in = 0;

    // 1) FIR anti-alias LP (you implement this)
    //    must return Q15
    y1 = x_in; // DUMMY DRY RUN NO FIR FILTER

    // 2) IIR high-pass (Direct Form I)
    acc  = (Int32)hp_b[0] * y1;
    acc += (Int32)hp_b[1] * hp_xz[0];
    acc += (Int32)hp_b[2] * hp_xz[1];
    acc -= (Int32)hp_a[1] * hp_yz[0];
    acc -= (Int32)hp_a[2] * hp_yz[1];
    y2 = sat_q15(acc >> 14);
    hp_xz[1] = hp_xz[0];
    hp_xz[0] = y1;
    hp_yz[1] = hp_yz[0];
    hp_yz[0] = y2;

    // 3) Drive-gain (Q0 * Q15 -> Q15 saturating)
    acc = (Int32)drive * y2;
    y2  = sat_q15(acc);

    //    Piecewise soft-clip on y2 (Q15)
    sign = (y2 >= 0) ? 1 : -1;
    u    = (y2 >= 0) ? y2 : -y2;

    if (u < TH1) {
        // v = 2 * y2
        acc = (Int32)y2 << 1;
        v   = sat_q15(acc);
    }
    else if (u < TH2) {
        // v = sign * [ (3 - (2 - 3*u)^2) / 3 ]
        Int32 t    = C2 - (((Int32)u * C3) >> 15);
        Int32 t2   = (t * t) >> 15;
        Int32 num  = C3 - t2;
        Int32 vabs = (num * INV3) >> 15;
        v = (sign > 0) ? (Int16)vabs : (Int16)-vabs;
    }
    else {
        // hard-limit to +/-1.0
        v = (sign > 0) ?  0x7FFF : (Int16)-0x8000;
    }
    y3 = v;

    // 4) IIR low-pass (Direct Form I)
    acc  = (Int32)lp_b[0] * y3;
    acc += (Int32)lp_b[1] * lp_xz[0];
    acc += (Int32)lp_b[2] * lp_xz[1];
    acc -= (Int32)lp_a[1] * lp_yz[0];
    acc -= (Int32)lp_a[2] * lp_yz[1];
    y4 = sat_q15(acc >> 14);
    lp_xz[1] = lp_xz[0];
    lp_xz[0] = y3;
    lp_yz[1] = lp_yz[0];
    lp_yz[0] = y4;

    // 5) Level-scale (Q15 * Q15 -> Q30 >>15 -> Q15)
    acc = (Int32)level * y4;
    acc >>= 15;
    return sat_q15(acc);
}
