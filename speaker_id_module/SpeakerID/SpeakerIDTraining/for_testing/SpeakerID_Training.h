/*
 * MATLAB Compiler: 6.2 (R2016a)
 * Date: Sun Dec 04 15:02:04 2016
 * Arguments: "-B" "macro_default" "-W" "lib:SpeakerID_Training" "-T"
 * "link:lib" "-d"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\SpeakerIDTraining\for_testing"
 * "-v" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\training_script.m" "-a"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\G729\vadG729.m" "-a"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\G729\vadInitCstParams.m" 
 */

#ifndef __SpeakerID_Training_h
#define __SpeakerID_Training_h 1

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

#ifdef EXPORTING_SpeakerID_Training
#define PUBLIC_SpeakerID_Training_C_API __global
#else
#define PUBLIC_SpeakerID_Training_C_API /* No import statement needed. */
#endif

#define LIB_SpeakerID_Training_C_API PUBLIC_SpeakerID_Training_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_SpeakerID_Training
#define PUBLIC_SpeakerID_Training_C_API __declspec(dllexport)
#else
#define PUBLIC_SpeakerID_Training_C_API __declspec(dllimport)
#endif

#define LIB_SpeakerID_Training_C_API PUBLIC_SpeakerID_Training_C_API


#else

#define LIB_SpeakerID_Training_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_SpeakerID_Training_C_API 
#define LIB_SpeakerID_Training_C_API /* No special import/export declaration */
#endif

extern LIB_SpeakerID_Training_C_API 
bool MW_CALL_CONV SpeakerID_TrainingInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_SpeakerID_Training_C_API 
bool MW_CALL_CONV SpeakerID_TrainingInitialize(void);

extern LIB_SpeakerID_Training_C_API 
void MW_CALL_CONV SpeakerID_TrainingTerminate(void);



extern LIB_SpeakerID_Training_C_API 
void MW_CALL_CONV SpeakerID_TrainingPrintStackTrace(void);

extern LIB_SpeakerID_Training_C_API 
bool MW_CALL_CONV mlxTraining_script(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                     *prhs[]);



extern LIB_SpeakerID_Training_C_API bool MW_CALL_CONV mlfTraining_script(mxArray* rootDirName, mxArray* nMixtures);

#ifdef __cplusplus
}
#endif
#endif
