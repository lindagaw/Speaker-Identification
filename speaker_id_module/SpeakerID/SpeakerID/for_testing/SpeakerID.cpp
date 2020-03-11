//
// MATLAB Compiler: 6.1 (R2015b)
// Date: Thu Dec 17 00:08:56 2015
// Arguments: "-B" "macro_default" "-W" "cpplib:SpeakerID" "-T" "link:lib" "-d"
// "C:\Dropbox\UVa_lab\KinectSpeakerID-offline\SpeakerID\for_testing" "-v"
// "C:\Dropbox\UVa_lab\KinectSpeakerID-offline\realtime_detection.m"
// "C:\Dropbox\UVa_lab\KinectSpeakerID-offline\training.m" 
//

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
bool MW_CALL_CONV mlxRealtime_detection(int nlhs, mxArray *plhs[], int nrhs, mxArray 
                                        *prhs[])
{
  return mclFeval(_mcr_inst, "realtime_detection", nlhs, plhs, nrhs, prhs);
}

LIB_SpeakerID_C_API 
bool MW_CALL_CONV mlxTraining(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "training", nlhs, plhs, nrhs, prhs);
}

LIB_SpeakerID_CPP_API 
void MW_CALL_CONV realtime_detection(int nargout, mwArray& speakerIds, mwArray& values, 
                                     const mwArray& fname, const mwArray& nSpeakers, 
                                     const mwArray& ubm, const mwArray& gmm, const 
                                     mwArray& sessionSize)
{
  mclcppMlfFeval(_mcr_inst, "realtime_detection", nargout, 2, 5, &speakerIds, &values, &fname, &nSpeakers, &ubm, &gmm, &sessionSize);
}

LIB_SpeakerID_CPP_API 
void MW_CALL_CONV training(int nargout, mwArray& nSpeakers, mwArray& nDims, mwArray& 
                           nMixtures, mwArray& ubm, mwArray& gmm, const mwArray& dirname)
{
  mclcppMlfFeval(_mcr_inst, "training", nargout, 5, 1, &nSpeakers, &nDims, &nMixtures, &ubm, &gmm, &dirname);
}

