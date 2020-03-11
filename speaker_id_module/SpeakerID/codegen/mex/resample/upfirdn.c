/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * upfirdn.c
 *
 * Code generation for function 'upfirdn'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "resample.h"
#include "upfirdn.h"
#include "FIRRateConverter.h"
#include "SystemCore.h"

/* Variable Definitions */
static emlrtRSInfo b_emlrtRSI = { 151, "upfirdn",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\signal\\eml\\upfirdn.m" };

static emlrtRSInfo c_emlrtRSI = { 154, "upfirdn",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\signal\\eml\\upfirdn.m" };

static emlrtRSInfo d_emlrtRSI = { 155, "upfirdn",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\signal\\eml\\upfirdn.m" };

/* Function Definitions */
void upfirdn(resampleStackData *SD, const emlrtStack *sp, const real_T x_in
             [882000], real_T y_out[320084])
{
  dspcodegen_FIRRateConverter s;
  int32_T i1;
  emlrtStack st;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &b_emlrtRSI;
  c_FIRRateConverter_FIRRateConve(&s);
  for (i1 = 0; i1 < 4; i1++) {
    memcpy(&SD->f0.x_in[i1 * 220941], &x_in[i1 * 220500], 220500U * sizeof
           (real_T));
    memset(&SD->f0.x_in[i1 * 220941 + 220500], 0, 441U * sizeof(real_T));
  }

  st.site = &c_emlrtRSI;
  SystemCore_step(&st, &s, SD->f0.x_in, SD->f0.y);
  st.site = &d_emlrtRSI;
  SystemCore_reset(&st, &s);
  for (i1 = 0; i1 < 4; i1++) {
    memcpy(&y_out[i1 * 80021], &SD->f0.y[i1 * 80160], 80021U * sizeof(real_T));
  }

  /* System object Destructor function: dsp.FIRRateConverter */
}

/* End of code generation (upfirdn.c) */
