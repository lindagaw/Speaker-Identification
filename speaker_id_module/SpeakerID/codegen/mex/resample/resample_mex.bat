@echo off
set MATLAB=C:\PROGRA~1\MATLAB\R2016a
set MATLAB_ARCH=win64
set MATLAB_BIN="C:\Program Files\MATLAB\R2016a\bin"
set ENTRYPOINT=mexFunction
set OUTDIR=.\
set LIB_NAME=resample_mex
set MEX_NAME=resample_mex
set MEX_EXT=.mexw64
call setEnv.bat
echo # Make settings for resample > resample_mex.mki
echo COMPILER=%COMPILER%>> resample_mex.mki
echo COMPFLAGS=%COMPFLAGS%>> resample_mex.mki
echo OPTIMFLAGS=%OPTIMFLAGS%>> resample_mex.mki
echo DEBUGFLAGS=%DEBUGFLAGS%>> resample_mex.mki
echo LINKER=%LINKER%>> resample_mex.mki
echo LINKFLAGS=%LINKFLAGS%>> resample_mex.mki
echo LINKOPTIMFLAGS=%LINKOPTIMFLAGS%>> resample_mex.mki
echo LINKDEBUGFLAGS=%LINKDEBUGFLAGS%>> resample_mex.mki
echo MATLAB_ARCH=%MATLAB_ARCH%>> resample_mex.mki
echo BORLAND=%BORLAND%>> resample_mex.mki
echo OMPFLAGS= >> resample_mex.mki
echo OMPLINKFLAGS= >> resample_mex.mki
echo EMC_COMPILER=msvc120>> resample_mex.mki
echo EMC_CONFIG=optim>> resample_mex.mki
"C:\Program Files\MATLAB\R2016a\bin\win64\gmake" -B -f resample_mex.mk
