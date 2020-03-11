/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * SystemCore.h
 *
 * Code generation for function 'SystemCore'
 *
 */

#ifndef SYSTEMCORE_H
#define SYSTEMCORE_H

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
extern void SystemCore_reset(const emlrtStack *sp, dspcodegen_FIRRateConverter
  *obj);
extern void SystemCore_step(const emlrtStack *sp, dspcodegen_FIRRateConverter
  *obj, const real_T varargin_1[883764], real_T varargout_1[320640]);

#endif

/* End of code generation (SystemCore.h) */
