#! /usr/bin/env bash

# Usage:
# 
#   ./build.sh
#   ./build.sh --help
#   ./build.sh --reset
#   ./build.sh --cmake-build <BuildType
# 
# Run without parameters to use the default build (Release)
# 
# Options:
# 
#   --help                          Shows this help text and exits
#   --reset                         Deletes the build directory
#   --cmake-build <BuildType>       Builds using the build type (Release | Debug | RelWithDebInfo | MinSizeRel)

set -e
set -o pipefail

SCRIPT_NAME="$0"
if [ "$1" == "--cmd" ]; then
    SCRIPT_NAME="build.cmd"
    shift
fi

show_help() {
    echo ""
    echo "Usage:"
    echo ""
    echo "  $SCRIPT_NAME"
    echo "  $SCRIPT_NAME --help"
    echo "  $SCRIPT_NAME --reset"
    echo "  $SCRIPT_NAME --cmake-build Debug"
    echo ""
    echo "Run without parameters to use the default build (Release)"
    echo ""
    echo "Options:"
    echo ""
    echo "  --help                          Shows this help text and exits"
    echo "  --reset                         Deletes the build directory"
    echo "  --cmake-build (Build Type)      Builds using the build type (Release | Debug | RelWithDebInfo | MinSizeRel)"
    echo ""
    exit 0
}

case "$1" in
    --help)
        show_help
        exit 0
        ;;
    --reset)
        echo "Deleting build directory..."
        rm -rf build
        echo "Build directory removed"
        exit 0
        ;;
    --cmake-build)
        if [ -z "$2" ]; then
            echo "Error: No build type provided for --cmake-build"
            echo "Usage: $SCRIPT_NAME --cmake-build Debug"
            exit 1
        fi
        BUILD_TYPE="$2"
        BUILD_DIR="build/$BUILD_TYPE"
        shift 2
        ;;
    "")
        BUILD_TYPE="Release"
        BUILD_DIR="build/Release"
        ;;
    *)
        echo "Unknown option: $1"
        echo "Use '$SCRIPT_NAME --help' for usage."
        exit 1
        ;;
esac

# Check cmake and ninja
if ! cmake --version >/dev/null 2>&1; then
    echo ""
    echo "Error: Couldn't find CMake. Make sure you have it downloaded and added to the PATH."
    echo ""
    echo "Download CMake from: https://cmake.org/download/"
    echo ""
    exit 1
fi
if ! ninja --version >/dev/null 2>&1; then
    echo ""
    echo "Error: Couldn't find Ninja. Make sure you have it downloaded and added to the PATH."
    echo "Download Ninja from: https://github.com/ninja-build/ninja/releases"
    echo ""
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Build
echo "Building with CMake and Ninja..."
echo "  Build Type: $BUILD_TYPE"
echo "  Build Dir: $BUILD_DIR"

echo ""
echo "============= Configuring ============="
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -G Ninja

echo ""
echo "============= Building ============="
cmake --build "$BUILD_DIR"

echo ""
echo "Build complete: $BUILD_TYPE"
echo "Executable files are located in:"
echo "  $SCRIPT_DIR/$BUILD_DIR/bin/"