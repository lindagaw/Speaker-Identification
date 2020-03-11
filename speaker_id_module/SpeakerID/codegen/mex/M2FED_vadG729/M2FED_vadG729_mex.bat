@echo off
set MATLAB=C:\PROGRA~1\MATLAB\R2016a
set MATLAB_ARCH=win64
set MATLAB_BIN="C:\Program Files\MATLAB\R2016a\bin"
set ENTRYPOINT=mexFunction
set OUTDIR=.\
set LIB_NAME=M2FED_vadG729_mex
set MEX_NAME=M2FED_vadG729_mex
set MEX_EXT=.mexw64
call setEnv.bat
echo # Make settings for M2FED_vadG729 > M2FED_vadG729_mex.mki
echo COMPILER=%COMPILER%>> M2FED_vadG729_mex.mki
echo COMPFLAGS=%COMPFLAGS%>> M2FED_vadG729_mex.mki
echo OPTIMFLAGS=%OPTIMFLAGS%>> M2FED_vadG729_mex.mki
echo DEBUGFLAGS=%DEBUGFLAGS%>> M2FED_vadG729_mex.mki
echo LINKER=%LINKER%>> M2FED_vadG729_mex.mki
echo LINKFLAGS=%LINKFLAGS%>> M2FED_vadG729_mex.mki
echo LINKOPTIMFLAGS=%LINKOPTIMFLAGS%>> M2FED_vadG729_mex.mki
echo LINKDEBUGFLAGS=%LINKDEBUGFLAGS%>> M2FED_vadG729_mex.mki
echo MATLAB_ARCH=%MATLAB_ARCH%>> M2FED_vadG729_mex.mki
echo BORLAND=%BORLAND%>> M2FED_vadG729_mex.mki
echo OMPFLAGS= >> M2FED_vadG729_mex.mki
echo OMPLINKFLAGS= >> M2FED_vadG729_mex.mki
echo EMC_COMPILER=msvc120>> M2FED_vadG729_mex.mki
echo EMC_CONFIG=optim>> M2FED_vadG729_mex.mki
"C:\Program Files\MATLAB\R2016a\bin\win64\gmake" -B -f M2FED_vadG729_mex.mk
