cd %~dp0
call activate base
python train_main.py %~1
timeout 30
