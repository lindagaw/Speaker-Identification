/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * M2FED_vadG729.h
 *
 * Code generation for function 'M2FED_vadG729'
 *
 */

#ifndef M2FED_VADG729_H
#define M2FED_VADG729_H

/* Include files */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mwmathutil.h"
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include "rtwtypes.h"
#include "M2FED_vadG729_types.h"

/* Function Declarations */
extern void AC_not_empty_init(void);
extern void HPF_not_empty_init(void);
extern void LEV1_not_empty_init(void);
extern void LEV2_not_empty_init(void);
extern void LPC2LSF_not_empty_init(void);
extern real32_T M2FED_vadG729(const emlrtStack *sp, const real_T speech[441]);

#ifdef __WATCOMC__

#pragma aux M2FED_vadG729 value [8087];

#endif

extern void VAD_var_param_not_empty_init(void);
extern void ZCD_not_empty_init(void);

#endif

/* End of code generation (M2FED_vadG729.h) */
