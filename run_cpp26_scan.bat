@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "C:\amp_demos\cpp26-diff\cpp26"
C:\CPP_STD_25-2\cpptest\cpptestcli.exe -config "builtin://Recommended Rules" -compiler vc_14_3-64 -report "C:\amp_demos\cpp26-diff\cpp26\report_msvc" -- cl.exe /std:c++latest /EHsc /I src /c src\sensor_data.cpp src\data_processor.cpp src\config_manager.cpp src\main.cpp
