cd %~dp0
call activate pcr_env
python train_SID_python.py %~1
timeout 6000
