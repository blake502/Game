@echo off
SetLocal EnableDelayedExpansion

echo Building testbed...

set input_files=

for /R %%f in (*.c) do ( set input_files=!input_files! %%f )

SetLocal DisableDelayedExpansion

set include_flags=-Isrc -I..\engine\src
set linker_flags=-L../bin/ -lengine.lib
set compiler_flags=-g

set defines=-DDEBUG -DS_IMPORT

set output_dir=..\bin\
set output_name=testbed
set output_type=.exe
set output_path=%output_dir%%output_name%%output_type%

chdir

if not exist %output_dir% mkdir %output_dir%

rem del /Q ..\%output_dir%*

clang %input_files% -o%output_path% %defines% %compiler_flags% %linker_flags% %include_flags%

if %errorlevel% neq 0 (echo Build failed! [Error: %errorlevel%] && exit)

echo Building testbed complete!