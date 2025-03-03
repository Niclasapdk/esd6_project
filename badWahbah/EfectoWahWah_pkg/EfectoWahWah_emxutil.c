/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * EfectoWahWah_emxutil.c
 *
 * Code generation for function 'EfectoWahWah_emxutil'
 *
 */

/* Include files */
#include "EfectoWahWah_emxutil.h"
#include "EfectoWahWah_types.h"
#include "rt_nonfinite.h"
#include <stdlib.h>
#include <string.h>

/* Function Definitions */
/*makes new data stucture based on newNumel, and if the old data should be freed or kept*/
void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel)        /* emxArray is a pointer to emxArray_real_T structure found in EfectoWahWah_types*/
{                                                                             /* old numel is the old number for the array size*/
  int i;              /*Index */
  int newNumel;       /*the new number for the array size*/
  void *newData;      /* Pointer to hold new data*/
  /* Ensure oldNumel is not negative*/
  if (oldNumel < 0) {
    oldNumel = 0;
  }
  /* Calculates the total number of elements in the array */
  /* By looping based on the amount of dimensions, and then calculate the size of each dimension */
  /* Example [3,4] then 3 for 1st dim and 4 for 2nd dim and 3*4=12 which is the total size*/
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }
  /*if the new array size is larger then allocated size then allocate more memory*/
  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    /*Allocates 16, if smaller then 16*/
    if (i < 16) {
      i = 16;
    }
    /*Increase the allocated size by times 2 until it is = or > then newnumel*/
    /*Cannot exceed 32 bit maximum value, therefore set i= max possible value for 32 bit singed integer*/
    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }
    /*The allocated new memory*/
    newData = malloc((unsigned int)i * sizeof(double));
    /*if the old data is not empty then copy data to the new memory */
    /*However if canfreedata is true then the old data is cleaned */
    if (emxArray->data != NULL) {
      memcpy(newData, emxArray->data, sizeof(double) * (unsigned int)oldNumel);
      if (emxArray->canFreeData) {
        free(emxArray->data);
      }
    }
    emxArray->data = (double *)newData;   /*Set pointer to new data location*/
    emxArray->allocatedSize = i;          /*Updata new size for allocated data*/
    emxArray->canFreeData = true;         /*free the old data if canfreedata is set true*/
  }
}

/*freeing memory allocated for emxArray_real_T*/
void emxFree_real_T(emxArray_real_T **pEmxArray)                               /*Uses a double pointer, done for ensuring all can be set to Null after pointing*/
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {                                 /*So it doesn't try to free memory which isn't allocated*/
    if (((*pEmxArray)->data != (double *)NULL) && (*pEmxArray)->canFreeData) { /*ensures there is allocated memory to free*/
      free((*pEmxArray)->data);                                                /*freeing data if flag is true, data*/
    }
    free((*pEmxArray)->size);                                                  /*can be freed if flag is true, size*/
    free(*pEmxArray);                                                          /*can be freed if flag is true, the array structure*/
    *pEmxArray = (emxArray_real_T *)NULL;                                      /*ensures it doesn't point to de allocated memory, so set to null*/
  }
}

/*An Intial data structure, which is an empty 2D array*/
/*double pointer used*/
void emxInit_real_T(emxArray_real_T **pEmxArray)
{
  emxArray_real_T *emxArray;                                              /*store the newly allocated structure*/
  int i;                                                                  /*index*/
  *pEmxArray = (emxArray_real_T *)malloc(sizeof(emxArray_real_T));        /*allocates the memory for the new structure, point to the location*/
  emxArray = *pEmxArray;                                                  /*assigns to be the pointer*/
  emxArray->data = (double *)NULL;                                        /*set data pointer to null as no data yet*/
  emxArray->numDimensions = 2;                                            /*Makes it a 2D array*/
  emxArray->size = (int *)malloc(sizeof(int) * 2U);                       /*stores the size each of the arrays dimensions*/
  emxArray->allocatedSize = 0;                                            /*Allocated size for data set to zero as no data yet*/
  emxArray->canFreeData = true;                                           /*should be freed when array is deleted*/
  for (i = 0; i < 2; i++) {                                               /*tells us that the array stucture is [0,0]*/
    emxArray->size[i] = 0;
  }
}

/* End of code generation (EfectoWahWah_emxutil.c) */
