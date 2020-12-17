cd %~dp0
call activate pcr_env
python SID_VAD.py %~1
timeout 30
if %errorlevel% neq 0 exit /b %errorlevel%
