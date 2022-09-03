cd %~dp0
call activate pcr_env
python main.py %~1
timeout 30
