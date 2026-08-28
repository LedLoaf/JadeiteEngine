@echo OFF

Title Building Emscripten Game
setlocal EnableDelayedExpansion

echo Running CMake...
:: Run Cmake and check for errors 
call emcmake cmake -S . -B build

if %errorlevel% neq 0 goto error 

echo Changing Directory...
:: Change directory and check for errors 
call cd build

echo Changed directory, error level: %errorlevel%
if %errorlevel% neq 0 goto error 

:: Run make and check for errors
call emmake make 
if %errorlevel% neq 0 goto error 

:: Success
echo Build Successful!
pause 

exit /b

:error
echo Build failed 
pause 

exit /b
