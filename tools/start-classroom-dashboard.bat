@echo off
cd /d "W:\soc3050code\tools"
echo Starting ATmega128 Classroom Dashboard...
echo Server: http://192.168.0.202:5001
echo.
python dashboards\multi_user_dashboard.py
pause
