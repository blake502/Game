SetLocal EnableDelayedExpansion

@echo off
echo Building engine...

set input_files=

for /R %%f in (*.c) do ( set input_files=!input_files! %%f )

SetLocal DisableDelayedExpansion

set include_flags=-I%vulkan_sdk%\include -Isrc
set linker_flags=-luser32 -lwinmm -lvulkan-1 -L%VULKAN_SDK%\Lib
set compiler_flags=-g -shared -Wvarargs
rem -Wall -Werror
set defines=-D_DEBUG -DS_EXPORT -D_CRT_SECURE_NO_WARNINGS

set output_dir=..\bin\
set output_name=engine
set output_type=.dll
set output_path=%output_dir%%output_name%%output_type%

chdir

if not exist %output_dir% mkdir %output_dir%

del /Q %output_dir%*

clang %input_files% -o%output_path% %defines% %compiler_flags% %linker_flags% %include_flags%

if %errorlevel% neq 0 (echo Build failed! [Error: %errorlevel%] && exit)

echo Building engine complete!