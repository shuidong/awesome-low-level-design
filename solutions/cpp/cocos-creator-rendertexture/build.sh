#!/bin/bash
# Build script for Cocos Creator 3.8 RenderTexture Example (Linux/macOS)
# Run this script from the project root directory

echo "Cocos Creator 3.8 RenderTexture Example Build Script"
echo "===================================================="

# Check if COCOS2DX_ROOT environment variable is set
if [ -z "$COCOS2DX_ROOT" ]; then
    echo "ERROR: COCOS2DX_ROOT environment variable is not set!"
    echo "Please set COCOS2DX_ROOT to point to your Cocos2d-x installation directory."
    echo "Example: export COCOS2DX_ROOT=/opt/cocos2d-x-4.0"
    exit 1
fi

echo "Using Cocos2d-x from: $COCOS2DX_ROOT"

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Configure CMake project
echo "Configuring CMake project..."
cd build

# Detect platform and set appropriate generator
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    cmake .. -G "Xcode"
else
    # Linux
    cmake .. -G "Unix Makefiles"
fi

if [ $? -ne 0 ]; then
    echo "ERROR: CMake configuration failed!"
    exit 1
fi

# Build the project
echo "Building project..."
cmake --build . --config Release

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed!"
    exit 1
fi

echo ""
echo "Build completed successfully!"

# Find the executable
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Executable location: build/Release/CocosRenderTextureExample"
else
    echo "Executable location: build/CocosRenderTextureExample"
fi

echo ""

# Option to run the example
read -p "Do you want to run the example now? (y/n): " choice
if [[ $choice == [Yy]* ]]; then
    echo "Running example..."
    if [[ "$OSTYPE" == "darwin"* ]]; then
        ./Release/CocosRenderTextureExample
    else
        ./CocosRenderTextureExample
    fi
fi

cd ..
echo "Done."