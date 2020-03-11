/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * BiquadFilter.c
 *
 * Code generation for function 'BiquadFilter'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "M2FED_vadG729.h"
#include "BiquadFilter.h"

/* Function Definitions */
dspcodegen_BiquadFilter *BiquadFilter_BiquadFilter(dspcodegen_BiquadFilter *obj)
{
  dspcodegen_BiquadFilter *b_obj;
  dspcodegen_BiquadFilter *c_obj;
  dsp_BiquadFilter_0 *d_obj;
  int32_T i;
  static const real_T dv1[3] = { 0.92727434635162354, -1.8544940948486328,
    0.92727434635162354 };

  b_obj = obj;
  c_obj = b_obj;
  c_obj->isInitialized = 0;
  d_obj = &b_obj->cSFunObject;

  /* System object Constructor function: dsp.BiquadFilter */
  d_obj->P0_ICRTP = 0.0;
  for (i = 0; i < 3; i++) {
    d_obj->P1_RTP1COEFF[i] = dv1[i];
  }

  for (i = 0; i < 2; i++) {
    d_obj->P2_RTP2COEFF[i] = -1.9059464931488037 + 2.8173488974571228 * (real_T)
      i;
  }

  for (i = 0; i < 2; i++) {
    d_obj->P3_RTP3COEFF[i] = 1.0 - (real_T)i;
  }

  for (i = 0; i < 2; i++) {
    d_obj->P4_RTP_COEFF3_BOOL[i] = false;
  }

  return b_obj;
}

/* End of code generation (BiquadFilter.c) */
