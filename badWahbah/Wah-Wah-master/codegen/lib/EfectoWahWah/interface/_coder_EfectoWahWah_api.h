/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_EfectoWahWah_api.h
 *
 * Code generation for function 'EfectoWahWah'
 *
 */

#ifndef _CODER_EFECTOWAHWAH_API_H
#define _CODER_EFECTOWAHWAH_API_H

/* Include files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
real_T EfectoWahWah(real_T damp, real_T minf, real_T maxf, real_T Fs, real_T Fw,
                    real_T x, real_T Fc, real_T wahType);

void EfectoWahWah_api(const mxArray *const prhs[8], const mxArray **plhs);

void EfectoWahWah_atexit(void);

void EfectoWahWah_initialize(void);

void EfectoWahWah_terminate(void);

void EfectoWahWah_xil_shutdown(void);

void EfectoWahWah_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/* End of code generation (_coder_EfectoWahWah_api.h) */
