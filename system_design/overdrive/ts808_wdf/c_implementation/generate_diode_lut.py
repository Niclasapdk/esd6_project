import numpy as np
import sys
from scipy.special import wrightomega

# -----------------------------
# User-configurable parameters:
# -----------------------------
TABLE_SIZE = 0x4000
A_MIN = -(1<<7)
A_MAX =   1<<7

# Circuit constants from your code (examples):
V_T = 0.026
# If you have R_Is = par_r * I_S, use that. For demonstration:
R_Is = 2.0e-6
# Then R_Is_over_Vt = R_Is / V_T:
R_Is_over_Vt = R_Is / V_T      # e.g. ~ 7.69e-5 / 0.026 => ...
# Similarly, if your code had a known "log_R_Is_over_Vt":
log_R_Is_over_Vt = np.log(R_Is_over_Vt)  # or a value like -12.222595...

# If your code uses signum:
def signum(x):
    return 1 if x > 0 else (-1 if x < 0 else 0)

def diode_better(a):
    """
    Python version of your DIODE_BETTER formula:

      int lambda = signum(a);
      TYPE lambda_a_over_vt = lambda * a / V_T;
      b = a - 2 * V_T * lambda * (
               omega4(log_R_Is_over_Vt + lambda_a_over_vt)
               - 
               omega4(log_R_Is_over_Vt - lambda_a_over_vt)
           );

    We'll do the same steps in Python using SciPy's Lambert W.
    """
    lam = signum(a)
    lambda_a_over_vt = lam * a / V_T

    # Evaluate the two omega4(...) calls
    w_plus  = wrightomega(log_R_Is_over_Vt + lambda_a_over_vt)
    w_minus = wrightomega(log_R_Is_over_Vt - lambda_a_over_vt)

    # Final b
    b = a - 2.0 * V_T * lam * ( w_plus - w_minus )
    return b

def diode_regular(a):
    """
    Python version of your "regular" diode model:

      int lambda = signum(a);
      TYPE other_part = (lambda * a)/V_T + R_Is_over_Vt;
      TYPE wright = omega4(log_R_Is_over_Vt + other_part);
      TYPE b = a + 2 * lambda * (R_Is - V_T * wright);

    """
    lam = signum(a)
    # (lambda * a)/V_T + R_Is_over_Vt
    other_part = (lam * a) / V_T + R_Is_over_Vt

    # wright = omega4(log_R_Is_over_Vt + other_part)
    wright = wrightomega(log_R_Is_over_Vt + other_part)

    # b = a + 2 * lambda * (R_Is - V_T * wright)
    b = a + 2.0 * lam * (R_Is - V_T * wright)
    return b

def float_to_q7_8(x):
    """
    Convert a float in approx [-127..+127] to Q15 in [-32768..+32767].
    Clamp if necessary.
    """
    if x/127 >  0.99997:
        x =  0.99997*127
    if x/127 < -1.0:
        x = -127.0
    return int(round(x * 32767.0 / 127))

def main():
    step = (A_MAX - A_MIN) / (TABLE_SIZE - 1)

    # Print the array definition
    print("#ifndef DIODE_H")
    print("#define DIODE_H")
    print("#include <stdint.h>")
    print(f"static const int16_t g_diode_table[{TABLE_SIZE}] = {{")
    hi = -999999999
    lo = 999999999

    for i in range(TABLE_SIZE):
        # Current "a" in float, covering [A_MIN..A_MAX]
        a_f = A_MIN + i * step

        # Evaluate advanced diode model
        b_f = diode_better(a_f)
        hi = max(b_f, hi)
        lo = min(b_f, lo)

        # If b_f might exceed ±1, consider scaling. For example:
        # scale = 0.5 if you expect up to ±2.0 in 'b_f', etc.
        scale = 1.0
        b_q7_8 = float_to_q7_8(b_f * scale)

        # Print in columns of 8 for readability
        if i % 8 == 0 and i != 0:
            print("\n   ", end="")
        print(f"{b_q7_8:6d},", end="")

    print("\n};")
    print(f"""
int16_t diode_lut(int16_t x) {{
	uint16_t idx = (((uint32_t)x + 32768)*({TABLE_SIZE}-1))/65536;
    if (idx>={TABLE_SIZE}) {{
        perror("out of bounds");
        exit(-1);
    }}
    return g_diode_table[idx];
}}""")
    print("#endif")
    print(f"Table size: {TABLE_SIZE}\nMemory: {TABLE_SIZE*2} bytes", file=sys.stderr)
    print(f"{hi=}, {lo=}", file=sys.stderr)

if __name__ == "__main__":
    main()
