@echo off
cd /d "C:\amp_demos\cpp26-diff\pre_cpp26"
set PATH=C:\msys64\ucrt64\bin;%PATH%
C:\CPP_STD_25-2\cpptest\cpptestcli.exe -config "builtin://Recommended Rules" -compiler gcc_11-64 -report "C:\amp_demos\cpp26-diff\pre_cpp26\report_gcc" -- g++.exe -std=c++20 -I src -c src/sensor_data.cpp src/data_processor.cpp src/config_manager.cpp src/main.cpp
