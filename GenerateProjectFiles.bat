@echo off
setlocal enabledelayedexpansion
echo.
echo ========================================
echo Unreal Engine 5.6 Project File Generator
echo ========================================
echo.

REM Get the directory where the script is located
set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=!SCRIPT_DIR:~0,-1!"

REM Search for .uproject file in the script directory
set "UPROJECT_FILE="
for %%F in ("!SCRIPT_DIR!\*.uproject") do (
    set "UPROJECT_FILE=%%~nxF"
    goto uproject_found
)

echo ERROR: No .uproject file found in script directory!
echo Script directory: !SCRIPT_DIR!
echo.
pause
exit /b 1

:uproject_found
echo Found project file: !UPROJECT_FILE!
echo.

:folderselect
echo Please enter the path to your UE 5.6 folder.
echo Example: C:\Program Files\Epic Games\UE_5.6
echo.
set /p UE_PATH="Enter UE 5.6 folder path: "

REM Remove quotes from the path (both single and double)
set "UE_PATH=!UE_PATH:"=!"
set "UE_PATH=!UE_PATH:'=!"

REM Trim trailing backslash if present
if "!UE_PATH:~-1!"=="\" set "UE_PATH=!UE_PATH:~0,-1!"

REM Check if the path exists
if not exist "!UE_PATH!" (
    echo.
    echo ERROR: Path does not exist: !UE_PATH!
    echo.
    set /p RETRY="Would you like to try again? (Y/N): "
    if /i "!RETRY!"=="Y" goto folderselect
    echo Cancelled.
    pause
    exit /b 1
)

REM Check if UnrealBuildTool.exe exists in the specified path
if not exist "!UE_PATH!\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" (
    echo.
    echo ERROR: UnrealBuildTool.exe not found in specified path!
    echo Expected location: !UE_PATH!\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe
    echo.
    set /p RETRY="Would you like to try again? (Y/N): "
    if /i "!RETRY!"=="Y" goto folderselect
    echo Cancelled.
    pause
    exit /b 1
)

echo.
echo Using UE 5.6 from: !UE_PATH!
echo Using project: !UPROJECT_FILE!
echo.

REM Run the command
echo Generating project files...
echo.
"!UE_PATH!\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="!SCRIPT_DIR!\!UPROJECT_FILE!" -game -progress

if errorlevel 1 (
    echo.
    echo ERROR: Project file generation failed!
    pause
    exit /b 1
) else (
    echo.
    echo SUCCESS: Project files generated!
    pause
)

endlocal