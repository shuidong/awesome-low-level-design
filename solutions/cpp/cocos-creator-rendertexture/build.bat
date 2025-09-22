@echo off
REM Build script for Cocos Creator 3.8 RenderTexture Example
REM Run this script from the project root directory

echo Cocos Creator 3.8 RenderTexture Example Build Script
echo ====================================================

REM Check if COCOS2DX_ROOT environment variable is set
if "%COCOS2DX_ROOT%"=="" (
    echo ERROR: COCOS2DX_ROOT environment variable is not set!
    echo Please set COCOS2DX_ROOT to point to your Cocos2d-x installation directory.
    echo Example: set COCOS2DX_ROOT=C:\cocos2d-x-4.0
    pause
    exit /b 1
)

echo Using Cocos2d-x from: %COCOS2DX_ROOT%

REM Create build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

REM Configure CMake project
echo Configuring CMake project...
cd build
cmake .. -G "Visual Studio 16 2019" -A x64

if %ERRORLEVEL% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release

if %ERRORLEVEL% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo Build completed successfully!
echo Executable location: build\bin\Release\CocosRenderTextureExample.exe
echo.

REM Option to run the example
set /p choice="Do you want to run the example now? (y/n): "
if /i "%choice%"=="y" (
    echo Running example...
    cd bin\Release
    CocosRenderTextureExample.exe
)

cd ..\..\..
echo Done.
pause