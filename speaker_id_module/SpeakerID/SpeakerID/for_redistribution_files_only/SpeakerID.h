/*
 * MATLAB Compiler: 6.2 (R2016a)
 * Date: Mon Jul 31 23:35:47 2017
 * Arguments: "-B" "macro_default" "-W" "lib:SpeakerID" "-T" "link:lib" "-d"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\SpeakerID\for_testing" "-v"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\M2FED_load_models.m"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\M2FED_realtime_detection.m"
 * "-a" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\resample_mex1.mexw64"
 * "-a" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\resample_mex2.mexw64"
 * "-a" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\resample_mex4.mexw64"
 * "-a" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\resample_mex8.mexw64"
 * "-a" "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\G729\vadG729.m" "-a"
 * "C:\Dropbox\UVa_lab\SpeakerIDMatlab\SpeakerID\G729\vadInitCstParams.m" 
 */

#ifndef __SpeakerID_h
#define __SpeakerID_h 1

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

#ifdef EXPORTING_SpeakerID
#define PUBLIC_SpeakerID_C_API __global
#else
#define PUBLIC_SpeakerID_C_API /* No import statement needed. */
#endif

#define LIB_SpeakerID_C_API PUBLIC_SpeakerID_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_SpeakerID
#define PUBLIC_SpeakerID_C_API __declspec(dllexport)
#else
#define PUBLIC_SpeakerID_C_API __declspec(dllimport)
#endif

#define LIB_SpeakerID_C_API PUBLIC_SpeakerID_C_API


#else

#define LIB_SpeakerID_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_SpeakerID_C_API 
#define LIB_SpeakerID_C_API /* No special import/export declaration */
#endif

extern LIB_SpeakerID_C_API 
bool MW_CALL_CONV SpeakerIDInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_SpeakerID_C_API 
bool MW_CALL_CONV SpeakerIDInitialize(void);

extern LIB_SpeakerID_C_API 
void MW_CALL_CONV SpeakerIDTerminate(void);



extern LIB_SpeakerID_C_API 
void MW_CALL_CONV SpeakerIDPrintStackTrace(void);

extern LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlxM2FED_load_models(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[]);

extern LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlxM2FED_realtime_detection(int nlhs, mxArray *plhs[], int nrhs, 
                                              mxArray *prhs[]);



extern LIB_SpeakerID_C_API bool MW_CALL_CONV mlfM2FED_load_models(int nargout, mxArray** models, mxArray** nSpeakers, mxArray** isOK, mxArray* rootDirName, mxArray* nMixtures);

extern LIB_SpeakerID_C_API bool MW_CALL_CONV mlfM2FED_realtime_detection(int nargout, mxArray** num_count, mxArray** speakerIds, mxArray* dataStream, mxArray* sampling_rate, mxArray* channels, mxArray* nSpeakers, mxArray* models, mxArray* session_speaking_percentage, mxArray* sessionSize, mxArray* gmm_threshold, mxArray* channel_map);

#ifdef __cplusplus
}
#endif
#endif
