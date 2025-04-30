#!/usr/bin/env python3
import sys
import math

# Python script to generate Q14 lookup tables for HP and LP biquads
# Usage: python3 make_biquad_luts_q14.py > biquad_luts_q14.h
# Comments printed to stderr; C arrays to stdout

print("# Q14 LUT generator for HP and LP 2nd-order Butterworth biquads", file=sys.stderr)
print("# Coefficients quantized to Q14 (±1.0 -> ±16384)", file=sys.stderr)
print("# Outputs C arrays for HP and LP filters to stdout", file=sys.stderr)

FS = 44100.0

# High-pass cutoff bins
hp_freqs = [100, 200, 300, 400, 500, 1000, 1500, 2000, 2500, 3000]
hp_b = []
hp_a = []

for fc in hp_freqs:
    Wn = math.tan(math.pi * fc / FS)
    D  = 1.0 + math.sqrt(2.0) * Wn + Wn * Wn

    # High-pass float biquad coefficients
    b0 =  1.0 / D
    b1 = -2.0 / D
    b2 =  1.0 / D
    a1 =  2.0 * (Wn * Wn - 1.0) / D
    a2 =  (1.0 - math.sqrt(2.0) * Wn + Wn * Wn) / D

    # Quantize to Q14
    hp_b.append([
        int(round(b0 * 16384)),
        int(round(b1 * 16384)),
        int(round(b2 * 16384))
    ])
    hp_a.append([
        16384,
        int(round(a1 * 16384)),
        int(round(a2 * 16384))
    ])

# Low-pass cutoff bins
lp_freqs = [4000, 5000, 6000, 7000, 8000, 9000, 10000]
lp_b = []
lp_a = []

for fc in lp_freqs:
    Wn = math.tan(math.pi * fc / FS)
    D  = 1.0 + math.sqrt(2.0) * Wn + Wn * Wn

    # Low-pass float biquad coefficients
    b0 = (Wn * Wn) / D
    b1 = 2.0 * (Wn * Wn) / D
    b2 = (Wn * Wn) / D
    a1 = 2.0 * (Wn * Wn - 1.0) / D
    a2 = (1.0 - math.sqrt(2.0) * Wn + Wn * Wn) / D

    # Quantize to Q14
    lp_b.append([
        int(round(b0 * 16384)),
        int(round(b1 * 16384)),
        int(round(b2 * 16384))
    ])
    lp_a.append([
        16384,
        int(round(a1 * 16384)),
        int(round(a2 * 16384))
    ])

# Print HP LUT
print(f"#define OD_HP_LUT_LEN ({len(hp_freqs)})")
print("static const Int16 hp_fc_list[] = {" + ", ".join(map(str, hp_freqs)) + "};")
print("static const Int16 hp_b_table[][3] = {")
for row in hp_b:
    print("    {" + ", ".join(f"{v}" for v in row) + "},")
print("};")
print("static const Int16 hp_a_table[][3] = {")
for row in hp_a:
    print("    {" + ", ".join(f"{v}" for v in row) + "},")
print("};")
print()

# Print LP LUT
print(f"#define OD_LP_LUT_LEN ({len(lp_freqs)})")
print("static const Int16 lp_fc_list[] = {" + ", ".join(map(str, lp_freqs)) + "};")
print("static const Int16 lp_b_table[][3] = {")
for row in lp_b:
    print("    {" + ", ".join(f"{v}" for v in row) + "},")
print("};")
print("static const Int16 lp_a_table[][3] = {")
for row in lp_a:
    print("    {" + ", ".join(f"{v}" for v in row) + "},")
print("};")

