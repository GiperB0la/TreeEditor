@echo off
setlocal

echo ========================================
echo        TreeEditor - MinGW Build
echo ========================================
echo.

set "QT=C:\Qt\6.10.3\mingw_64"
set "MINGW=C:\Qt\Tools\mingw1310_64"

echo [1/5] Checking MinGW...

if not exist "%MINGW%\bin\g++.exe" (
    echo ERROR: MinGW not found:
    echo %MINGW%\bin\g++.exe
    pause
    exit /b 1
)

echo MinGW: %MINGW%
echo.

echo [2/5] Checking Qt...

if not exist "%QT%\bin\qmake.exe" (
    echo ERROR: Qt MinGW not found:
    echo %QT%
    pause
    exit /b 1
)

echo Qt: %QT%
echo.

echo [3/5] Preparing environment...

set "PATH=%MINGW%\bin;%QT%\bin;%PATH%"

echo Compiler:
g++ --version
echo.

echo [4/5] Cleaning build directory...

if exist build (
    rmdir /s /q build
)

mkdir build
cd build

echo.
echo [5/5] Configuring CMake...

cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="%QT%"

if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed.
    pause
    exit /b 1
)

echo.
echo ========================================
echo              BUILDING
echo ========================================
echo.

cmake --build . -j

if errorlevel 1 (
    echo.
    echo ERROR: Build failed.
    pause
    exit /b 1
)

echo.
echo ========================================
echo          BUILD SUCCESSFUL
echo ========================================
echo.
echo Executable:
echo %cd%\TreeEditor.exe
echo.

pause