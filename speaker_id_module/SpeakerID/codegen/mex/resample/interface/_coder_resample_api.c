/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_resample_api.c
 *
 * Code generation for function '_coder_resample_api'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "resample.h"
#include "_coder_resample_api.h"
#include "resample_data.h"

/* Function Declarations */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[882000];
static const mxArray *b_emlrt_marshallOut(const real_T u[8821]);
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[882000];
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x, const
  char_T *identifier))[882000];
static const mxArray *emlrt_marshallOut(const real_T u[320000]);

/* Function Definitions */
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[882000]
{
  real_T (*y)[882000];
  y = c_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}
  static const mxArray *b_emlrt_marshallOut(const real_T u[8821])
{
  const mxArray *y;
  const mxArray *m2;
  static const int32_T iv4[2] = { 0, 0 };

  static const int32_T iv5[2] = { 1, 8821 };

  y = NULL;
  m2 = emlrtCreateNumericArray(2, iv4, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m2, (void *)u);
  emlrtSetDimensions((mxArray *)m2, iv5, 2);
  emlrtAssign(&y, m2);
  return y;
}

static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[882000]
{
  real_T (*ret)[882000];
  static const int32_T dims[2] = { 220500, 4 };

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims);
  ret = (real_T (*)[882000])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
  static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *x, const
  char_T *identifier))[882000]
{
  real_T (*y)[882000];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  y = b_emlrt_marshallIn(sp, emlrtAlias(x), &thisId);
  emlrtDestroyArray(&x);
  return y;
}

static const mxArray *emlrt_marshallOut(const real_T u[320000])
{
  const mxArray *y;
  const mxArray *m1;
  static const int32_T iv2[2] = { 0, 0 };

  static const int32_T iv3[2] = { 80000, 4 };

  y = NULL;
  m1 = emlrtCreateNumericArray(2, iv2, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m1, (void *)u);
  emlrtSetDimensions((mxArray *)m1, iv3, 2);
  emlrtAssign(&y, m1);
  return y;
}

void resample_api(resampleStackData *SD, const mxArray * const prhs[3], const
                  mxArray *plhs[2])
{
  real_T (*yout)[320000];
  real_T (*h)[8821];
  static const uint32_T p_in[4] = { 3113836974U, 2582488612U, 3706583155U,
    1356491448U };

  static const uint32_T q_in[4] = { 3071801933U, 2878074626U, 3124171067U,
    1534726900U };

  real_T (*x)[882000];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  yout = (real_T (*)[320000])mxMalloc(sizeof(real_T [320000]));
  h = (real_T (*)[8821])mxMalloc(sizeof(real_T [8821]));

  /* Check constant function inputs */
  emlrtCheckArrayChecksumR2014a(&st, "p_in", p_in, prhs[1], false);
  emlrtCheckArrayChecksumR2014a(&st, "q_in", q_in, prhs[2], false);

  /* Marshall function inputs */
  x = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "x");

  /* Invoke the target function */
  resample(SD, &st, *x, *yout, *h);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(*yout);
  plhs[1] = b_emlrt_marshallOut(*h);
}

/* End of code generation (_coder_resample_api.c) */
