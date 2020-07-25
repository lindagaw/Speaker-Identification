cd %~dp0
call activate base
python main.py %~1
timeout 30
