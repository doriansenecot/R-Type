#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status
set -o pipefail  # Prevents errors in a pipeline from being masked

# Set the vcpkg path and toolchain file
VCPKG_PATH="$HOME/vcpkg"
TOOLCHAIN_FILE="$VCPKG_PATH/scripts/buildsystems/vcpkg.cmake"

# Check if vcpkg is installed
if [ ! -d "$VCPKG_PATH" ]; then
  echo "vcpkg not found in $VCPKG_PATH. Please install vcpkg first."
  exit 1
fi

# Install dependencies via vcpkg
echo "Installing dependencies via vcpkg..."
$VCPKG_PATH/vcpkg install boost-system boost-filesystem boost-date-time boost-serialization sfml asio --triplet x64-linux

# Remove old build directory if it exists
echo "Cleaning previous build artifacts..."
rm -rf build
mkdir build

# Run CMake to configure the project
echo "Configuring project with CMake..."
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DCMAKE_BUILD_TYPE=Release

# Compile the project
echo "Building project..."
cmake --build build --config Release

echo "Build completed successfully!"
