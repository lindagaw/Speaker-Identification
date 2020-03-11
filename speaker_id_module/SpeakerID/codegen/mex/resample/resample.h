/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * resample.h
 *
 * Code generation for function 'resample'
 *
 */

#ifndef RESAMPLE_H
#define RESAMPLE_H

/* Include files */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include "rtwtypes.h"
#include "resample_types.h"

/* Function Declarations */
extern void resample(resampleStackData *SD, const emlrtStack *sp, const real_T
                     x[882000], real_T yout[320000], real_T h[8821]);

#endif

/* End of code generation (resample.h) */
