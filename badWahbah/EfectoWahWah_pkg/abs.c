/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * abs.c
 *
 * Code generation for function 'abs'
 *
 */

/* Include files */
#include "abs.h"
#include "EfectoWahWah_emxutil.h"
#include "EfectoWahWah_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */

/* Take x the input array and computes the absolut value and stores it in y*/
void b_abs(const emxArray_real_T *x, emxArray_real_T *y)
{
  const double *x_data;               /*Pointer to the input data*/
  double *y_data;                     /*Pointer to output*/
  int k;                              /*Calculated to be previous total number of elements in output array lin 30*/
  int nx_tmp;                         /*The column size of X array */
  x_data = x->data;                   /*stores the pointer to x into x_data*/
  nx_tmp = x->size[1];                /*Number of columns in input array size[1]=column, size[0]=row assumed to be 1-row matrix*/
  k = y->size[0] * y->size[1];        /*calculate previous array size of y, to for resizing in emxEnsureCapacity_real_T*/
  y->size[0] = 1;                     /*set is so that it is a 1-row matrix*/
  y->size[1] = x->size[1];            /*Makes it so it column size is the same as x input array*/
  emxEnsureCapacity_real_T(y, k);     /*Calls this function to allocated the need amount of allocate space*/
  y_data = y->data;                   /*stores the pointer to y into y_data*/
  for (k = 0; k < nx_tmp; k++) {      /*Goes over element in x and calculate the absolut value of each*/
    y_data[k] = fabs(x_data[k]);
  }
}

/* End of code generation (abs.c) */
