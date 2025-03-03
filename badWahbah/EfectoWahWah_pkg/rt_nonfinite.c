/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * rt_nonfinite.c
 *
 * Code generation for function 'EfectoWahWah'
 *
 */

/*
 * Abstract:
 *      MATLAB for code generation function to initialize non-finites,
 *      (Inf, NaN and -Inf).
 */
/* Include files */
#include "rt_nonfinite.h"
#include <math.h>

#if defined(__ICL) && __ICL == 1700
#pragma warning(disable : 264)
#endif

real_T rtNaN = (real_T)NAN;                 /* not a number (nan)*/
real_T rtInf = (real_T)INFINITY;            /* infinite minus check*/
real_T rtMinusInf = -(real_T)INFINITY;      /* infinite minus check*/
real32_T rtNaNF = (real32_T)NAN;            /* not a number (nan)*/
real32_T rtInfF = (real32_T)INFINITY;       /* infinite check*/
real32_T rtMinusInfF = -(real32_T)INFINITY; /* infinite minus check*/

#if defined(__ICL) && __ICL == 1700
#pragma warning(default : 264)
#endif

/*
 * Function: rtIsInf ==================================================
 *  Test if value is infinite
 *  isinf (math.h function) checks if it is infinite returns 0 if it finite isinf(5)=0
 *  if infinite it returns number which isnt 0
 *  !=0U checks if the value is 0, if it is 0 then returns false
 *  double = 64 bit
 */
boolean_T rtIsInf(real_T value)
{
  return (isinf(value) != 0U);
}

/*
 * Function: rtIsInfF =================================================
 *  Abstract:
 *  Test if single-precision value is infinite
 *  isinf (math.h function) checks if it is infinite returns 0 if it finite isinf(5)=0
 *  if infinite it returns number which isnt 0
 *  !=0U checks if the value is 0, if it is 0 then returns false
 *  single = 32 bit
 */
boolean_T rtIsInfF(real32_T value)
{
  return (isinf((real_T)value) != 0U);
}

/*
 * Function: rtIsNaN ==================================================
 *  Abstract:
 *  Test if value is not a number
 *  isnan (math.h function) checks if nan(not a number)
 *  If it is a number return 0, false
 *  if not a number then return 1, right
 *  double = 64 bit
 */
boolean_T rtIsNaN(real_T value)
{
  return (isnan(value) != 0U);
}

/*
 * Function: rtIsNaNF =================================================
 *  Abstract:
 *  Test if single-precision value is not a number
 * isnan (math.h function) checks if nan(not a number)
 * If it is a number return 0, false
 *  if not a number then return 1, right
 * single = 32 bit
 */
boolean_T rtIsNaNF(real32_T value)
{
  return (isnan((real_T)value) != 0U);
}

/* End of code generation (rt_nonfinite.c) */
