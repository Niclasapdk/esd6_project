#!/usr/bin/env python3
import math

# Scale factors
outputScale = 32768  # Q15

LUT_len = 36 #ca 0.1 sec interval

# LUT input range (floating point)
x_min = 0.5
x_max = 4.0

step = (x_max - x_min) / (LUT_len - 1)
print(step);exit()

lut = []

for i in range(LUT_len):
    x = x_min + i * step
    g = 1-(0.366 / x)
    q15_output = int(round(g * outputScale))
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
