@echo off

echo Building...

set include_flags=-I%Vulkan_SDK%\include
set linker_flags=-luser32
set output_dir=bin\
set output_name=main
set output_type=.exe

if not exist %output_dir% mkdir %output_dir%

clang src\*.c %linker_flags% %include_flags% -o%output_dir%%output_name%%output_type%

echo Building complete!