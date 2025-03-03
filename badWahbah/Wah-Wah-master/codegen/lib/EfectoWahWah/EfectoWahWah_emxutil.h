/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * EfectoWahWah_emxutil.h
 *
 * Code generation for function 'EfectoWahWah_emxutil'
 *
 */

#ifndef EFECTOWAHWAH_EMXUTIL_H
#define EFECTOWAHWAH_EMXUTIL_H

/* Include files */
#include "EfectoWahWah_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);

extern void emxFree_real_T(emxArray_real_T **pEmxArray);

extern void emxInit_real_T(emxArray_real_T **pEmxArray);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (EfectoWahWah_emxutil.h) */
