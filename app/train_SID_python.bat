cd %~dp0
call activate pcr_env
python train_SID_python.py %~1
timeout 30
if %errorlevel% neq 0 exit /b %errorlevel%
