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
/* Finds the smallest value in array x, and returns its index and pointer to it *idx */
double minimum(const emxArray_real_T *x, int *idx)
{
  const double *x_data;                                                 /*Pointer to the input data*/
  double ex;                                                            /*min value holder*/
  int k;                                                                /*loop counter for x array [k-1]*/
  int last_tmp;                                                         /*Number of columns in array x*/
  x_data = x->data;                                                     /*stores the pointer to x into x_data*/
  last_tmp = x->size[1];                                                /*Number of columns in input array size[1]=column, size[0]=row assumed to be 1-row matrix*/
  if (x->size[1] <= 2) {                                                /*If the array is 2 or 1 in size*/
    if (x->size[1] == 1) {                                              /*If it only has one value then return that as the smallest*/
      ex = x_data[0];                                                   /*min value ex is the 1st element*/                                   
      *idx = 1;                                                         /*Point to 1st element, min*/
    } else {
      ex = x_data[x->size[1] - 1];                                       /*Assume last elements is min*/
      if ((x_data[0] > ex) || (rtIsNaN(x_data[0]) && (!rtIsNaN(ex)))) {  /*If the first element is larger then the last, 2nd be min*/
        *idx = x->size[1];                                               /*points to 2nd element, min*/
      } else {
        ex = x_data[0];                                                  /*when first element isn't larger then last, 1st be min*/
        *idx = 1;                                                        /*point to 1st element, min*/
      }
    }
  } else {                                                               /*If there is more then 2 values in the x array*/
    if (!rtIsNaN(x_data[0])) {                                           /*If the 1st elemnt is not an NaN(not a number),so a number*/
      *idx = 1;   
    } else {                                                             /*If the 1st element is an NaN(not a number),  */
      boolean_T exitg1;
      *idx = 0;                                                          /*0 as no value is found yet*/
      k = 2;                                                             /*set k = 2, 2nd element*/
      exitg1 = false;                                                    /*set to false to run the while statement*/
      while ((!exitg1) && (k <= last_tmp)) {                             /*Loops until a number is found or end of array*/
        if (!rtIsNaN(x_data[k - 1])) {                                   /*check if number (non NaN)*/
          *idx = k;                                                      /*if a number store pointer to the index of the array*/
          exitg1 = true;                                                 /*Stop while statement cause number found*/
        } else {
          k++;                                                           /*check next value of array to find number (non NaN)*/
        }
      }
    }
    if (*idx == 0) {                                                    /*If all elements are NaN(not a number), set the 1st element to min*/
      ex = x_data[0];
      *idx = 1;
    } else {
      int i;
      ex = x_data[*idx - 1];                                            /*The min value in the x array, so far (the first number)(non NaN)*/
      i = *idx + 1;                                                     /*index for pointer*/
      for (k = i; k <= last_tmp; k++) {                                 /*Loops through until found min*/
        double d;
        d = x_data[k - 1];                                              /*The next value in the x array*/
        if (ex > d) {                                                   /*Check if the value is lower then the already min (ex)*/
          ex = d;                                                       /*If lower then set new min value and idx pointer*/
          *idx = k;
        }
      }
    }
  }
  return ex;                                                            /*Return min value*/
}

/* End of code generation (minOrMax.c) */
