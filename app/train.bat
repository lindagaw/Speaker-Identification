cd %~dp0
call activate pcr_env
python train_SID_main.py %~1
timeout 50
