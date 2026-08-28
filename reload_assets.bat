:: Reload Assets Batch File
@echo off

Title Reloading Assets
setlocal EnableExtensions EnableDelayedExpansion

:: ============= CONFIG ===============
set DATA_NAME=game.data
set PRELOAD_PATH=assets@/assets
set PACKAGER=file_packager.py
set JS_OUTPUT=%DATA_NAME%.js
set BUILD_PATH=build
:: ====================================

call python %EMSDK_TOOLS%/%PACKAGER% %DATA_NAME% --js-output=%JS_OUTPUT% --preload %PRELOAD_PATH% --from-emcc

if exist %DATA_NAME% (
	echo [INFO] Generated data files successfully
	
	move /Y "%DATA_NAME%" "%BUILD_PATH%" >nul
	move /Y "%JS_OUTPUT%" "%BUILD_PATH%" >nul
	
	echo [DONE] %DATA_NAME% and %JS_OUTPUT% updated and deployed.
) else (
	echo [ERROR] Failed to reload game assets.
	pause
)

endlocal
pause

exit /b