/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * minOrMax.c
 *
 * Code generation for function 'minOrMax'
 *
 */

/* Include files */
#include "minOrMax.h"
#include "EfectoWahWah_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

/* Function Definitions */
double minimum(const emxArray_real_T *x, int *idx)
{
  const double *x_data;
  double ex;
  int k;
  int last_tmp;
  x_data = x->data;
  last_tmp = x->size[1];
  if (x->size[1] <= 2) {
    if (x->size[1] == 1) {
      ex = x_data[0];
      *idx = 1;
    } else {
      ex = x_data[x->size[1] - 1];
      if ((x_data[0] > ex) || (rtIsNaN(x_data[0]) && (!rtIsNaN(ex)))) {
        *idx = x->size[1];
      } else {
        ex = x_data[0];
        *idx = 1;
      }
    }
  } else {
    if (!rtIsNaN(x_data[0])) {
      *idx = 1;
    } else {
      boolean_T exitg1;
      *idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= last_tmp)) {
        if (!rtIsNaN(x_data[k - 1])) {
          *idx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }
    if (*idx == 0) {
      ex = x_data[0];
      *idx = 1;
    } else {
      int i;
      ex = x_data[*idx - 1];
      i = *idx + 1;
      for (k = i; k <= last_tmp; k++) {
        double d;
        d = x_data[k - 1];
        if (ex > d) {
          ex = d;
          *idx = k;
        }
      }
    }
  }
  return ex;
}

/* End of code generation (minOrMax.c) */
