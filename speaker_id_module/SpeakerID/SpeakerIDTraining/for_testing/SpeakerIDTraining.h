/*
 * MATLAB Compiler: 6.2 (R2016a)
 * Date: Wed Aug 09 14:31:44 2017
 * Arguments: "-B" "macro_default" "-W" "lib:SpeakerIDTraining" "-T" "link:lib"
 * "-d"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\SpeakerIDTraining\for_testing"
 * "-v" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\M2FED_training_script.m"
 * "-a" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\G729\vadG729.m" "-a"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\G729\vadInitCstParams.m" 
 */

#ifndef __SpeakerIDTraining_h
#define __SpeakerIDTraining_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_SpeakerIDTraining
#define PUBLIC_SpeakerIDTraining_C_API __global
#else
#define PUBLIC_SpeakerIDTraining_C_API /* No import statement needed. */
#endif

#define LIB_SpeakerIDTraining_C_API PUBLIC_SpeakerIDTraining_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_SpeakerIDTraining
#define PUBLIC_SpeakerIDTraining_C_API __declspec(dllexport)
#else
#define PUBLIC_SpeakerIDTraining_C_API __declspec(dllimport)
#endif

#define LIB_SpeakerIDTraining_C_API PUBLIC_SpeakerIDTraining_C_API


#else

#define LIB_SpeakerIDTraining_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_SpeakerIDTraining_C_API 
#define LIB_SpeakerIDTraining_C_API /* No special import/export declaration */
#endif

extern LIB_SpeakerIDTraining_C_API 
bool MW_CALL_CONV SpeakerIDTrainingInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_SpeakerIDTraining_C_API 
bool MW_CALL_CONV SpeakerIDTrainingInitialize(void);

extern LIB_SpeakerIDTraining_C_API 
void MW_CALL_CONV SpeakerIDTrainingTerminate(void);



extern LIB_SpeakerIDTraining_C_API 
void MW_CALL_CONV SpeakerIDTrainingPrintStackTrace(void);

extern LIB_SpeakerIDTraining_C_API 
bool MW_CALL_CONV mlxM2FED_training_script(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                           *prhs[]);



extern LIB_SpeakerIDTraining_C_API bool MW_CALL_CONV mlfM2FED_training_script(mxArray* rootDirName, mxArray* nMixtures);

#ifdef __cplusplus
}
#endif
#endif
