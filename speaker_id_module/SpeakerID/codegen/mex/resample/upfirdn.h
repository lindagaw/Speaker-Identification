/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * upfirdn.h
 *
 * Code generation for function 'upfirdn'
 *
 */

#ifndef UPFIRDN_H
#define UPFIRDN_H

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
extern void upfirdn(resampleStackData *SD, const emlrtStack *sp, const real_T
                    x_in[882000], real_T y_out[320084]);

#endif

/* End of code generation (upfirdn.h) */
