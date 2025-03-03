/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * EfectoWahWah.c
 *
 * Code generation for function 'EfectoWahWah'
 *
 */

/* Include files */
#include "EfectoWahWah.h"
#include "EfectoWahWah_emxutil.h"
#include "EfectoWahWah_types.h"
#include "abs.h"
#include "colon.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <emmintrin.h>
#include <math.h>

/* Function Definitions */
double EfectoWahWah(double damp, double minf, double maxf, double Fs, double Fw,
                    double x, double Fc, double wahType)
{
  emxArray_real_T *b_freq;
  emxArray_real_T *freq;
  emxArray_real_T *r;
  double y;
  double *b_freq_data;
  double *freq_data;
  int i;
  int loop_ub;
  (void)damp;
  /*  Wah type = 1, is the triangle wave */
  /*  Wah type = 2, is the constant Fc */
  /*  Fc needs to be in the range if not then end */
  emxInit_real_T(&freq);
  freq_data = freq->data;
  emxInit_real_T(&b_freq);
  emxInit_real_T(&r);
  if ((Fc > maxf) || (Fc < minf)) {
    y = 0.0;
  } else {
    switch ((int)wahType) {
    case 1:
      /*  triangle wave / wahwah effect, making the wah wah sound */
      /*  tunning coeffiecients and start Fc */
      /*  first element in the vector */
      y = 2.0 * sin(3.1415926535897931 * minf / Fs) * x;
      /*  step size */
      break;
    case 2: {
      int scalarLB;
      int vectorUB;
      /*  for constant Fc same loop as shown above for triangle */
      y = Fw / Fs;
      /*  step size */
      /*  vector with all possible frequencies */
      if (rtIsNaN(minf) || rtIsNaN(y) || rtIsNaN(maxf)) {
        i = freq->size[0] * freq->size[1];
        freq->size[0] = 1;
        freq->size[1] = 1;
        emxEnsureCapacity_real_T(freq, i);
        freq_data = freq->data;
        freq_data[0] = rtNaN;
      } else if ((y == 0.0) || ((minf < maxf) && (y < 0.0)) ||
                 ((maxf < minf) && (y > 0.0))) {
        freq->size[0] = 1;
        freq->size[1] = 0;
      } else if ((rtIsInf(minf) || rtIsInf(maxf)) &&
                 (rtIsInf(y) || (minf == maxf))) {
        i = freq->size[0] * freq->size[1];
        freq->size[0] = 1;
        freq->size[1] = 1;
        emxEnsureCapacity_real_T(freq, i);
        freq_data = freq->data;
        freq_data[0] = rtNaN;
      } else if (rtIsInf(y)) {
        i = freq->size[0] * freq->size[1];
        freq->size[0] = 1;
        freq->size[1] = 1;
        emxEnsureCapacity_real_T(freq, i);
        freq_data = freq->data;
        freq_data[0] = minf;
      } else if ((floor(minf) == minf) && (floor(y) == y)) {
        i = freq->size[0] * freq->size[1];
        freq->size[0] = 1;
        loop_ub = (int)((maxf - minf) / y);
        freq->size[1] = loop_ub + 1;
        emxEnsureCapacity_real_T(freq, i);
        freq_data = freq->data;
        for (i = 0; i <= loop_ub; i++) {
          freq_data[i] = minf + y * (double)i;
        }
      } else {
        eml_float_colon(minf, y, maxf, freq);
        freq_data = freq->data;
      }
      /*  find the closest frequency to Fc */
      i = b_freq->size[0] * b_freq->size[1];
      b_freq->size[0] = 1;
      loop_ub = freq->size[1];
      b_freq->size[1] = freq->size[1];
      emxEnsureCapacity_real_T(b_freq, i);
      b_freq_data = b_freq->data;
      scalarLB = (freq->size[1] / 2) << 1;
      vectorUB = scalarLB - 2;
      for (i = 0; i <= vectorUB; i += 2) {
        __m128d r1;
        r1 = _mm_loadu_pd(&freq_data[i]);
        _mm_storeu_pd(&b_freq_data[i], _mm_sub_pd(r1, _mm_set1_pd(Fc)));
      }
      for (i = scalarLB; i < loop_ub; i++) {
        b_freq_data[i] = freq_data[i] - Fc;
      }
      b_abs(b_freq, r);
      minimum(r, &loop_ub);
      /*  idx gives the index of the closest value */
      /*  This is the closest frequency to Fc (finite length) */
      /*  tunning coeffiecients and start Fc */
      /*  first element in the vector */
      y = 2.0 * sin(3.1415926535897931 * freq_data[loop_ub - 1] / Fs) * x;
    } break;
    }
    /*  needs to do both */
    /*  normalize the values in the vector of y */
    y /= fabs(y);
  }
  emxFree_real_T(&r);
  emxFree_real_T(&b_freq);
  emxFree_real_T(&freq);
  return y;
}

/* End of code generation (EfectoWahWah.c) */
