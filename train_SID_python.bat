cd %~dp0
call activate base
python train_SID_python.py %~1
timeout 30
if %errorlevel% neq 0 exit /b %errorlevel%
