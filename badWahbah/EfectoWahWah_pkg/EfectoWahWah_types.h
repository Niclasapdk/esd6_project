/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * EfectoWahWah_types.h
 *
 * Code generation for function 'EfectoWahWah'
 *
 */

#ifndef EFECTOWAHWAH_TYPES_H
#define EFECTOWAHWAH_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  double *data;             /*stores the data of the array */
  int *size;                /*size of the array
                            * myArray.size[0] = 3; 3 rows 
                            * myArray.size[1] = 3; 3 columns*/
  int allocatedSize;        /*Total number of elements */
  int numDimensions;        /*The dimmensions amount Dim=2, 2D and Dim = 3, 3D*/
  boolean_T canFreeData;    /* Allows the data to be cleaned/remove when no longer needed*/
};
#endif /* struct_emxArray_real_T */

/*tells what should be done when the struct is defined with values */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

#endif
/* End of code generation (EfectoWahWah_types.h) */
