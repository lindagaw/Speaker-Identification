/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_resample_mex.c
 *
 * Code generation for function '_coder_resample_mex'
 *
 */

/* Include files */
#include "resample.h"
#include "_coder_resample_mex.h"
#include "resample_terminate.h"
#include "_coder_resample_api.h"
#include "resample_initialize.h"
#include "resample_data.h"

/* Variable Definitions */
static resampleStackData *resampleStackDataGlobal = NULL;

/* Function Declarations */
static void resample_mexFunction(resampleStackData *SD, int32_T nlhs, mxArray
  *plhs[2], int32_T nrhs, const mxArray *prhs[3]);

/* Function Definitions */
static void resample_mexFunction(resampleStackData *SD, int32_T nlhs, mxArray
  *plhs[2], int32_T nrhs, const mxArray *prhs[3])
{
  int32_T n;
  const mxArray *inputs[3];
  const mxArray *outputs[2];
  int32_T b_nlhs;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs < 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooFewInputsConstants", 9, 4, 8,
                        "resample", 4, 8, "resample", 4, 8, "resample");
  }

  if (nrhs != 3) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 3, 4, 8,
                        "resample");
  }

  if (nlhs > 2) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 8,
                        "resample");
  }

  /* Temporary copy for mex inputs. */
  for (n = 0; n < nrhs; n++) {
    inputs[n] = prhs[n];
    if (*emlrtBreakCheckR2012bFlagVar != 0) {
      emlrtBreakCheckR2012b(&st);
    }
  }

  /* Call the function. */
  resample_api(SD, inputs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);

  /* Module termination. */
  resample_terminate();
}

void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  resampleStackDataGlobal = (resampleStackData *)mxCalloc(1, 1U * sizeof
    (resampleStackData));
  mexAtExit(resample_atexit);

  /* Initialize the memory manager. */
  /* Module initialization. */
  resample_initialize();

  /* Dispatch the entry-point. */
  resample_mexFunction(resampleStackDataGlobal, nlhs, plhs, nrhs, prhs);
  mxFree(resampleStackDataGlobal);
}

emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/* End of code generation (_coder_resample_mex.c) */
