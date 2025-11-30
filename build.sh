#! /usr/bin/env bash

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
    echo "  $SCRIPT_NAME [options]"
    echo ""
    echo "Run without parameters to use the default build (Release)"
    echo ""
    echo "Options:"
    echo ""
    echo "  -h, --help                      Shows this help text and exits"
    echo "  -r, --reset                     Deletes the build directory"
    echo "  -b, --cmake-build <build_type>  Builds using the build type (Release | Debug | RelWithDebInfo | MinSizeRel)"
    echo "  -v, --verbose                   Uses verbose output"
    echo ""
    exit 0
}

BUILD_TYPE="Release"
BUILD_DIR="build/Release"
while [ $# -gt 0 ]; do
    case "$1" in
    "-v" | "--verbose")
        CONFIGURE_VERBOSE_FLAG="--log-level=VERBOSE"
        BUILD_VERBOSE_FLAG="-v"
        shift
        ;;
    "--help" | "-h")
        show_help
        exit 0
        ;;
    "--reset" | "-r")
        echo "Deleting build directory..."
        rm -rf build
        echo "Build directory removed"
        exit 0
        ;;
    "--cmake-build" | "-b")
        if [ -z "$2" ]; then
            echo "Error: No build type provided for --cmake-build"
            echo "Usage: $SCRIPT_NAME --cmake-build Debug"
            exit 1
        fi
        BUILD_TYPE="$2"
        BUILD_DIR="build/$BUILD_TYPE"
        shift 2
        ;;
    *)
        echo "Unknown option: $1"
        echo "Use '$SCRIPT_NAME --help' for usage."
        exit 1
        ;;
    esac
done

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
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -G Ninja $CONFIGURE_VERBOSE_FLAG

echo ""
echo "============= Building ============="
cmake --build "$BUILD_DIR" $BUILD_VERBOSE_FLAG

echo ""
echo "Build complete: $BUILD_TYPE"
echo "Executable files are located in:"
echo "  $SCRIPT_DIR/$BUILD_DIR/bin/"
