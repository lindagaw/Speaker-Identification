/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * log10.c
 *
 * Code generation for function 'log10'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "vadG729.h"
#include "log10.h"
#include "error.h"

/* Variable Definitions */
static emlrtRSInfo ab_emlrtRSI = { 13, "log10",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\elfun\\log10.m"
};

/* Function Definitions */
void b_log10(const emlrtStack *sp, real32_T *x)
{
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  if (*x < 0.0F) {
    st.site = &ab_emlrtRSI;
    error(&st);
  }

  *x = muSingleScalarLog10(*x);
}

/* End of code generation (log10.c) */
