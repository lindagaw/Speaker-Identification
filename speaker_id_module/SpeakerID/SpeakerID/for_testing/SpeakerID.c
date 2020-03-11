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

#include <stdio.h>
#define EXPORTING_SpeakerID 1
#include "SpeakerID.h"

static HMCRINSTANCE _mcr_inst = NULL;


#if defined( _MSC_VER) || defined(__BORLANDC__) || defined(__WATCOMC__) || defined(__LCC__)
#ifdef __LCC__
#undef EXTERN_C
#endif
#include <windows.h>

static char path_to_dll[_MAX_PATH];

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, void *pv)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (GetModuleFileName(hInstance, path_to_dll, _MAX_PATH) == 0)
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
    }
    return TRUE;
}
#endif
#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_SpeakerID_C_API
#define LIB_SpeakerID_C_API /* No special import/export declaration */
#endif

LIB_SpeakerID_C_API 
bool MW_CALL_CONV SpeakerIDInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
  if (!GetModuleFileName(GetModuleHandle("SpeakerID"), path_to_dll, _MAX_PATH))
    return false;
    {
        mclCtfStream ctfStream = 
            mclGetEmbeddedCtfStream(path_to_dll);
        if (ctfStream) {
            bResult = mclInitializeComponentInstanceEmbedded(   &_mcr_inst,
                                                                error_handler, 
                                                                print_handler,
                                                                ctfStream);
            mclDestroyStream(ctfStream);
        } else {
            bResult = 0;
        }
    }  
    if (!bResult)
    return false;
  return true;
}

LIB_SpeakerID_C_API 
bool MW_CALL_CONV SpeakerIDInitialize(void)
{
  return SpeakerIDInitializeWithHandlers(mclDefaultErrorHandler, mclDefaultPrintHandler);
}

LIB_SpeakerID_C_API 
void MW_CALL_CONV SpeakerIDTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_SpeakerID_C_API 
void MW_CALL_CONV SpeakerIDPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(&stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlxM2FED_load_models(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                       *prhs[])
{
  return mclFeval(_mcr_inst, "M2FED_load_models", nlhs, plhs, nrhs, prhs);
}

LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlxM2FED_realtime_detection(int nlhs, mxArray *plhs[], int nrhs, 
                                              mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "M2FED_realtime_detection", nlhs, plhs, nrhs, prhs);
}

LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlfM2FED_load_models(int nargout, mxArray** models, mxArray** 
                                       nSpeakers, mxArray** isOK, mxArray* rootDirName, 
                                       mxArray* nMixtures)
{
  return mclMlfFeval(_mcr_inst, "M2FED_load_models", nargout, 3, 2, models, nSpeakers, isOK, rootDirName, nMixtures);
}

LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlfM2FED_realtime_detection(int nargout, mxArray** num_count, mxArray** 
                                              speakerIds, mxArray* dataStream, mxArray* 
                                              sampling_rate, mxArray* channels, mxArray* 
                                              nSpeakers, mxArray* models, mxArray* 
                                              session_speaking_percentage, mxArray* 
                                              sessionSize, mxArray* gmm_threshold, 
                                              mxArray* channel_map)
{
  return mclMlfFeval(_mcr_inst, "M2FED_realtime_detection", nargout, 2, 9, num_count, speakerIds, dataStream, sampling_rate, channels, nSpeakers, models, session_speaking_percentage, sessionSize, gmm_threshold, channel_map);
}

