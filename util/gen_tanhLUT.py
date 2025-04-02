#!/usr/bin/env python3
import math

# Scale factors
outputScale = 32768  # Q15

tanhLUT_len = 1024

# LUT input range (floating point)
x_min = -3.0
x_max =  3.0

step = (x_max - x_min) / (tanhLUT_len - 1)

lut = []

for i in range(tanhLUT_len):
    x = x_min + i * step
    y = math.tanh(x)
    q15_output = int(round(y * outputScale))
    if q15_output > 32767:
        q15_output = 32767
    if q15_output < -32768:
        print("Error: q15_output below valid range")
        exit(-1)
    lut.append(q15_output)

print(f"#define tanhLUT_len {tanhLUT_len}")
print("static const Int16 tanhLUT[tanhLUT_len] = {")
for i, val in enumerate(lut):
    if i % 8 == 0:
        print("   ", end="")
    print("{:6d}".format(val), end="")
    if i < tanhLUT_len - 1:
        print(", ", end="")
    if i % 8 == 7:
        print("")
print("\n};")
