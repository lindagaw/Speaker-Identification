/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * resample_types.h
 *
 * Code generation for function 'resample'
 *
 */

#ifndef RESAMPLE_TYPES_H
#define RESAMPLE_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_dsp_FIRRateConverter_0
#define struct_dsp_FIRRateConverter_0

struct dsp_FIRRateConverter_0
{
  int32_T S0_isInitialized;
  real_T W0_InBuf[232];
  int32_T W1_InBufIdx;
  real_T P0_FILTER[9280];
  int32_T P1_PolyphaseSelector[160];
  int32_T P2_StartIdx[441];
  int32_T P3_StopIdx[441];
};

#endif                                 /*struct_dsp_FIRRateConverter_0*/

#ifndef typedef_dsp_FIRRateConverter_0
#define typedef_dsp_FIRRateConverter_0

typedef struct dsp_FIRRateConverter_0 dsp_FIRRateConverter_0;

#endif                                 /*typedef_dsp_FIRRateConverter_0*/

#ifndef typedef_dspcodegen_FIRRateConverter
#define typedef_dspcodegen_FIRRateConverter

typedef struct {
  int32_T isInitialized;
  dsp_FIRRateConverter_0 cSFunObject;
} dspcodegen_FIRRateConverter;

#endif                                 /*typedef_dspcodegen_FIRRateConverter*/

#ifndef typedef_resampleStackData
#define typedef_resampleStackData

typedef struct {
  struct {
    real_T x_in[883764];
    real_T y[320640];
  } f0;

  struct {
    real_T y[320084];
  } f1;
} resampleStackData;

#endif                                 /*typedef_resampleStackData*/
#endif

/* End of code generation (resample_types.h) */
