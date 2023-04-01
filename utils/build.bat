@echo off

echo Building...

set include_flags=-I%vulkan_sdk%\include
set linker_flags=-luser32 -lwinmm
set output_dir=bin\
set output_name=main
set output_type=.exe
set output_path=%output_dir%%output_name%%output_type%

if not exist %output_dir% mkdir %output_dir%

del /Q %output_dir%\*

clang src\*.c %linker_flags% %include_flags% -o%output_path%

if %errorlevel% neq 0 (echo Build failed! [Error: %errorlevel%] && exit)

echo Building complete!