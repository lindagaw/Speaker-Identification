/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * M2FED_vadG729_terminate.c
 *
 * Code generation for function 'M2FED_vadG729_terminate'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "M2FED_vadG729.h"
#include "M2FED_vadG729_terminate.h"
#include "_coder_M2FED_vadG729_mex.h"
#include "M2FED_vadG729_data.h"

/* Function Definitions */
void M2FED_vadG729_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  emlrtDestroyArray(&eml_mx);
  emlrtDestroyArray(&b_eml_mx);
}

void M2FED_vadG729_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (M2FED_vadG729_terminate.c) */
