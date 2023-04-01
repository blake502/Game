@echo off
SetLocal EnableDelayedExpansion

echo Building...

set input_files=

for /R %%f in (*.c) do ( set input_files=!input_files! %%f )

set include_flags=-I%vulkan_sdk%\include -Isrc\
set linker_flags=-luser32 -lwinmm
set compiler_flags=-g
set output_dir=bin\
set output_name=main
set output_type=.exe
set output_path=%output_dir%%output_name%%output_type%

if not exist %output_dir% mkdir %output_dir%

del /Q %output_dir%\*

clang %input_files% %compiler_flags% %linker_flags% %include_flags% -o%output_path%

if %errorlevel% neq 0 (echo Build failed! [Error: %errorlevel%] && exit)

echo Building complete!