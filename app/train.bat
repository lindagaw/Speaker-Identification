cd %~dp0
call activate base
python train_SID_main.py %~1
timeout 6000
