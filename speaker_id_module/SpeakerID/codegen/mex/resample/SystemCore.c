/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * SystemCore.c
 *
 * Code generation for function 'SystemCore'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "resample.h"
#include "SystemCore.h"

/* Variable Definitions */
static emlrtRSInfo h_emlrtRSI = { 1, "SystemCore",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\shared\\system\\coder\\+matlab\\+system\\+coder\\SystemCore.p"
};

static emlrtRSInfo i_emlrtRSI = { 1, "Nondirect",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\shared\\system\\coder\\+matlab\\+system\\+mixin\\+coder\\Nondirect.p"
};

static emlrtRTEInfo emlrtRTEI = { 1, 1, "SystemCore",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\shared\\system\\coder\\+matlab\\+system\\+coder\\SystemCore.p"
};

/* Function Definitions */
void SystemCore_reset(const emlrtStack *sp, dspcodegen_FIRRateConverter *obj)
{
  dspcodegen_FIRRateConverter *b_obj;
  int32_T i;
  if (obj->isInitialized != 2) {
  } else {
    emlrtErrorWithMessageIdR2012b(sp, &emlrtRTEI,
      "MATLAB:system:methodCalledWhenReleasedCodegen", 3, 4, 5, "reset");
  }

  if (obj->isInitialized == 1) {
    b_obj = obj;

    /* System object Initialization function: dsp.FIRRateConverter */
    b_obj->cSFunObject.W1_InBufIdx = 0;
    for (i = 0; i < 232; i++) {
      b_obj->cSFunObject.W0_InBuf[i] = 0.0;
    }
  }
}

void SystemCore_step(const emlrtStack *sp, dspcodegen_FIRRateConverter *obj,
                     const real_T varargin_1[883764], real_T varargout_1[320640])
{
  dspcodegen_FIRRateConverter *b_obj;
  dsp_FIRRateConverter_0 *c_obj;
  int32_T outIdx;
  int32_T inIdx;
  int32_T m;
  int32_T i;
  int32_T inBufIdx;
  int32_T inputChannelOffset;
  int32_T n;
  int32_T outputStartIdx;
  int32_T outputStopIdx;
  real_T acc;
  int32_T coefPolyphaseOffset;
  real_T prod;
  emlrtStack st;
  emlrtStack b_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  if (obj->isInitialized != 2) {
  } else {
    emlrtErrorWithMessageIdR2012b(sp, &emlrtRTEI,
      "MATLAB:system:methodCalledWhenReleasedCodegen", 3, 4, 4, "step");
  }

  if (obj->isInitialized != 1) {
    st.site = &h_emlrtRSI;
    b_obj = obj;
    b_st.site = &h_emlrtRSI;
    if (b_obj->isInitialized == 0) {
    } else {
      emlrtErrorWithMessageIdR2012b(&b_st, &emlrtRTEI,
        "MATLAB:system:methodCalledWhenLockedReleasedCodegen", 3, 4, 5, "setup");
    }

    b_obj->isInitialized = 1;
    b_st.site = &h_emlrtRSI;

    /* System object Initialization function: dsp.FIRRateConverter */
    b_obj->cSFunObject.W1_InBufIdx = 0;
    for (i = 0; i < 232; i++) {
      b_obj->cSFunObject.W0_InBuf[i] = 0.0;
    }
  }

  st.site = &h_emlrtRSI;
  b_obj = obj;
  b_st.site = &i_emlrtRSI;
  c_obj = &b_obj->cSFunObject;

  /* System object Outputs function: dsp.FIRRateConverter */
  outIdx = 0;
  inIdx = 0;
  for (m = 0; m < 4; m++) {
    /* Update inBufIdx and inputChannelOffset for current channel */
    inBufIdx = c_obj->W1_InBufIdx;
    inputChannelOffset = m * 58;
    for (n = 0; n < 220941; n++) {
      i = n % 441;
      outputStartIdx = c_obj->P2_StartIdx[i];
      outputStopIdx = c_obj->P3_StopIdx[i];

      /* Read input into inBufArray */
      c_obj->W0_InBuf[inBufIdx + inputChannelOffset] = varargin_1[inIdx];
      inIdx++;

      /* Generate outputs (if any) for current input n */
      while (outputStartIdx < outputStopIdx) {
        acc = 0.0;
        coefPolyphaseOffset = c_obj->P1_PolyphaseSelector[outputStartIdx] * 58;
        for (i = inBufIdx; i < 58; i++) {
          prod = c_obj->P0_FILTER[(coefPolyphaseOffset + i) - inBufIdx] *
            c_obj->W0_InBuf[i + inputChannelOffset];
          acc += prod;
        }

        for (i = 0; i < inBufIdx; i++) {
          prod = c_obj->P0_FILTER[((coefPolyphaseOffset - inBufIdx) + i) + 58] *
            c_obj->W0_InBuf[i + inputChannelOffset];
          acc += prod;
        }

        varargout_1[outIdx] = acc;
        outIdx++;
        outputStartIdx++;
      }

      /* Decrement inBufIdx, wrap if necessary */
      if (inBufIdx == 0) {
        inBufIdx = 57;
      } else {
        inBufIdx--;
      }
    }
  }

  /* Update inBufIdx */
  b_obj->cSFunObject.W1_InBufIdx = inBufIdx;
}

/* End of code generation (SystemCore.c) */
