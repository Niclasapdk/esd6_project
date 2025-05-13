#!/usr/bin/env python3
import math

# Scale factors
outputScale = 32768  # Q15

LUT_len = 36 #ca 0.1 sec interval

# LUT input range (floating point)
t_min = 0.5
t_max = 4.0

step = (t_max - t_min) / (LUT_len - 1)

lut = []

def check_stability(g):
    g1 = [0.46, 0.48, 0.50, 0.52, 0.53, 0.55]
    g2 = [g*(1-g1) for g1 in g1]
    for g1, g2 in zip(g1, g2):
        print("fuck", g1, g2, g1+g2, g)
        if g1+g2 >= 1:
            exit(-1)

for i in range(LUT_len):
    t = t_min + i * step
    g = 1-(0.366 / t)
    check_stability(g)
    q15_output = int(math.floor(g * outputScale))
    #q15_output = int(y*10)
    if q15_output > 32767:
        print("Error: q15_output above valid range")
        exit(-1)
    if q15_output < -32768:
        print("Error: q15_output below valid range")
        exit(-1)
    lut.append(q15_output)

print(f"#define reverbgain_LUT_len {LUT_len}")
print("static const Int16 reverbgain_LUT[reverbgain_LUT_len] = {")
for i, val in enumerate(lut):
    if i % 8 == 0:
        print("   ", end="")
    print("{:6d}".format(val), end="")
    if i < LUT_len - 1:
        print(", ", end="")
    if i % 8 == 7:
        print("")
print("\n};")
