#ifndef OMEGA_H
#define OMEGA_H

#ifndef TYPE
#define TYPE float
#endif

/*
   Evaluate a 3rd‑order polynomial using Estrin’s scheme.
   Given coefficients arranged as
       { a3, a2, a1, a0 }
   this computes
       a3*x^3 + a2*x^2 + a1*x + a0.
*/
static inline TYPE estrin3(const TYPE coeffs[4], TYPE x)
{
    /* Compute in two groups:
         t0 = a0 + a1*x
         t1 = a2 + a3*x
       then combine as: t0 + t1*x^2
       (note: with the ordering below this gives:
         result = coeffs[3] + coeffs[2]*x + coeffs[1]*x^2 + coeffs[0]*x^3)
    */
    TYPE t0 = coeffs[3] + coeffs[2] * x;
    TYPE t1 = coeffs[1] + coeffs[0] * x;
    return t0 + t1 * (x * x);
}

/* Approximation of log2(x) on x in [1,2]:
   Evaluates a polynomial
       P(x) = alpha*x^3 + beta*x^2 + gamma*x + zeta
   with coefficients from the original code.
*/
static inline TYPE log2_approx(TYPE x)
{
    const TYPE coeffs[4] = {
        0.1640425613334452f,   // coefficient for x^3
        -1.098865286222744f,     // coefficient for x^2
        3.148297929334117f,     // coefficient for x^1
        -2.213475204444817f      // constant term
    };
    return estrin3(coeffs, x);
}

/* Approximation of natural logarithm.
   The method “extracts” the exponent from the float bit‐pattern,
   normalizes the mantissa to [1,2) and then computes:
      ln(x) = ln(2) * ( e + log2_approx(mantissa) )
*/
static inline TYPE log_approx(TYPE x)
{
    union {
        int i;
        TYPE f;
    } v;
    v.f = x;
    int ex = v.i & 0x7f800000;          // extract exponent bits
    int e = (ex >> 23) - 127;             // compute unbiased exponent
    /* Normalize mantissa: clear exponent and set to 1.xxx */
    v.i = (v.i - ex) | 0x3f800000;
    return 0.693147180559945f * ( (TYPE)e + log2_approx(v.f) );
}

/* Approximation of 2^x for x in [0,1]:
   Evaluates a polynomial
       P(x) = alpha*x^3 + beta*x^2 + gamma*x + zeta
   with coefficients as given.
*/
static inline TYPE pow2_approx(TYPE x)
{
    const TYPE coeffs[4] = {
        0.07944154167983575f,  // coefficient for x^3
        0.2274112777602189f,   // coefficient for x^2
        0.6931471805599453f,   // coefficient for x^1
        1.0f                   // constant term
    };
    return estrin3(coeffs, x);
}

/* Approximation of exp(x):
   First the argument is clamped (so that the exponent is not too negative),
   then we “split” x into an integer part l and a fractional part f.
   The integer part is used to construct the float from its bits,
   while the fractional part is approximated via pow2_approx.
*/
static inline TYPE exp_approx(TYPE x)
{
    if (x < -126.0f)
        x = -126.0f;
    int xi = (int)x;
    int l = (x < 0.0f) ? xi - 1 : xi;
    TYPE f = x - (TYPE)l;
    union {
        int i;
        TYPE f;
    } v;
    v.i = (l + 127) << 23;
    return v.f * pow2_approx(f);
}

/* First-order approximation:
   Simply returns x if x>=0, otherwise 0.
*/
TYPE omega1(TYPE x)
{
    return (x < 0 ? 0 : x);
}

/* Second-order approximation:
   For x below x1, return 0.
   For x above x2, return x.
   Otherwise use a 3rd‑order polynomial.
*/
TYPE omega2(TYPE x)
{
    const TYPE x1 = -3.684303659906469f;
    const TYPE x2 =  1.972967391708859f;
    const TYPE a = 9.451797158780131e-3f;
    const TYPE b = 1.126446405111627e-1f;
    const TYPE c = 4.451353886588814e-1f;
    const TYPE d = 5.836596684310648e-1f;

    if (x < x1)
        return 0;
    if (x > x2)
        return x;
    const TYPE coeffs[4] = { a, b, c, d };
    return estrin3(coeffs, x);
}

/* Third-order approximation:
   For x below x1, return 0.
   For x between x1 and x2, evaluate a polynomial.
   For x above x2, return x - log_approx(x).
*/
TYPE omega3(TYPE x)
{
    const TYPE x1 = -3.341459552768620f;
    const TYPE x2 = 8.0f;
    const TYPE a = -1.314293149877800e-3f;
    const TYPE b = 4.775931364975583e-2f;
    const TYPE c = 3.631952663804445e-1f;
    const TYPE d = 6.313183464296682e-1f;

    if (x < x1)
        return 0;
    if (x < x2) {
        const TYPE coeffs[4] = { a, b, c, d };
        return estrin3(coeffs, x);
    }
    return x - log_approx(x);
}

/* Fourth-order approximation:
   Compute y = omega3(x) and then “correct” it with an exponential term.
*/
TYPE omega4(TYPE x)
{
    TYPE y = omega3(x);
    return y - (y - exp_approx(x - y)) / (y + 1);
}

#endif //OMEGA_H
