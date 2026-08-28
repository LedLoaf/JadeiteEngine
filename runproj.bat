@echo off
Title Starting Emscripten
setlocal

call cd build

echo starting game

:: Start the first html file found
for %%f in (*.html) do (
	echo Running %%f
	call emrun "%%f"
	endlocal
	exit /b
)

echo No HTML file found!
pause
endlocal
