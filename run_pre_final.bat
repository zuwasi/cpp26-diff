@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat" >nul 2>&1
cd /d "C:\amp_demos\cpp26-diff\pre_cpp26"
C:\CPP_STD_25-2\cpptest\cpptestcli.exe -config "builtin://Flow Analysis Aggressive" -compiler vc_14_3-64 -report "C:\amp_demos\cpp26-diff\pre_cpp26\report_final" -property report.format=html+sarif -property flowanalysis.depth=deep -- cl.exe /std:c++20 /EHsc /W4 /I src /c src\sensor_data.cpp src\data_processor.cpp src\config_manager.cpp src\main.cpp
