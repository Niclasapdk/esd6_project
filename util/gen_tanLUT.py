#!/usr/bin/env python3
import math

# scale factors
Q1_14_SCALE = 16384
Q15_SCALE = 32768

# Length of the lookup table
tanhLUT_len = 1024

# Define the full Q15 input range
int16_min = -32768
int16_max = 32767

# Compute the step so that the table spans from q15_min to q15_max
step = (int16_max - int16_min) / (tanhLUT_len - 1)

lut = []

for i in range(tanhLUT_len):
    # Determine the Q15 input value (as an integer)
    x = int(round(int16_min + i * step))
    
    # Convert the Q15 input to a floating-point number in [-1, ~1)
    x = x / Q1_14_SCALE
    
    # Compute the tanh of the floating-point value
    y = math.tanh(x)
    
    # Convert the result back to Q15
    q15_output = int(round(y * Q15_SCALE))
    
    # Saturate (Q15 valid range is from -32768 to 32767)
    if q15_output > 32767:
        print("fuck")
        print(q15_output)
        exit(-1)
    if q15_output < -32768:
        print("fuck")
        print(q15_output)
        exit(-1)
    
    lut.append(q15_output)

# Print the lookup table as a C-style array
print(f"#define tanhLUT_len {tanhLUT_len}")
print("static const Int16 tanhLUT[tanhLUT_len] = {")
for i, val in enumerate(lut):
    # Print 8 values per line for readability
    if i % 8 == 0:
        print("   ", end="")
    print("{:6d}".format(val), end="")
    if i < tanhLUT_len - 1:
        print(", ", end="")
    if i % 8 == 7:
        print("")
print("\n};")
