@echo off

REM Set the vcpkg path and toolchain file
set VCPKG_PATH=%USERPROFILE%\vcpkg
set TOOLCHAIN_FILE=%VCPKG_PATH%\scripts\buildsystems\vcpkg.cmake

REM Check if vcpkg is installed
if not exist "%VCPKG_PATH%" (
    echo vcpkg not found in %VCPKG_PATH%. Please install vcpkg first.
    exit /b 1
)

REM Install dependencies via vcpkg
echo Installing dependencies via vcpkg...
"%VCPKG_PATH%\vcpkg" install boost-system boost-filesystem boost-date-time boost-serialization sfml asio --triplet x64-windows

REM Remove old build directory if it exists
if exist build (
    rmdir /S /Q build
)
mkdir build

REM Run CMake to configure the project
echo Configuring project with CMake...
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAIN_FILE% -DCMAKE_BUILD_TYPE=Release

REM Compile the project
echo Building project...
cmake --build build --config Release

echo Build completed successfully!
