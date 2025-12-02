#! /usr/bin/env bash

set -e
set -o pipefail

SCRIPT_NAME="$0"
if [ "$1" = "--cmd" ]; then
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
    echo "  -m, --msvc <version>            Uses MSVC (Visual Studio) to build"
    echo "                                  Run without a version to see available versions"
    echo "  -l, --msvc-legacy               For use with Visual Studio 2019, 16.11.0 - 16.11.13"
    echo "  -b, --cmake-build <build_type>  Builds using the build type (Release | Debug | RelWithDebInfo | MinSizeRel)"
    echo "  -v, --verbose                   Uses verbose output"
    echo ""
    exit 0
}

GENERATOR="Ninja"
MAKE_PROGRAM="Ninja"
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
    "--msvc" | "-m")
        if [ -z "$2" ]; then
            echo "Usage: $SCRIPT_NAME --msvc <version>"
            echo ""
            echo "Available Versions:"
            echo "  2019"
            echo "  2022"
            echo "  2026"
            exit 0
        fi
        case "$2" in
        "2019")
            GENERATOR="Visual Studio 16 2019"
            ;;
        "2022")
            GENERATOR="Visual Studio 17 2022"
            ;;
        "2026")
            GENERATOR="Visual Studio 18 2026"
            ;;
        *)
            echo "Error: Invalid Visual Studio Version: $2"
            echo ""
            echo "Available Versions:"
            echo "  2019"
            echo "  2022"
            echo "  2026"
            exit 1
            ;;
        esac
        MSVC=1
        MAKE_PROGRAM="Visual Studio"
        shift 2
        ;;
    "--msvc-legacy" | "-l")
        GENERATOR="Visual Studio 16 2019"
        MAKE_PROGRAM="Visual Studio"
        MSVC=1
        MSVC_LEGACY=1
        shift
        ;;

    "--cmake-build" | "-b")
        if [ -z "$2" ]; then
            echo "Error: No build type provided for --cmake-build"
            echo "Usage: $SCRIPT_NAME --cmake-build <build_type>"
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

# Check cmake
if ! cmake --version >/dev/null 2>&1; then
    echo ""
    echo "Error: Couldn't find CMake. Make sure you have it downloaded and added to the PATH."
    echo ""
    echo "Download CMake from: https://cmake.org/download/"
    echo ""
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Build
echo "Building with CMake and $MAKE_PROGRAM..."
echo "  Build Type: $BUILD_TYPE"
echo "  Build Dir: $BUILD_DIR"

echo ""
echo "============= Configuring ============="
CONFIGURE_OPTIONS=("-S" "$SCRIPT_DIR" "-B" "$BUILD_DIR" "-DCMAKE_BUILD_TYPE=$BUILD_TYPE" "-G" "$GENERATOR")
if [ -n "$CONFIGURE_VERBOSE_FLAG" ]; then
    CONFIGURE_OPTIONS+=("$CONFIGURE_VERBOSE_FLAG")
fi
if [ -n "$MSVC_LEGACY" ]; then
    CONFIGURE_OPTIONS+=("-DMSVC_LEGACY=1")
fi
cmake "${CONFIGURE_OPTIONS[@]}" || (./build.sh -r && exit 1)

echo ""
echo "============= Building ============="
BUILD_OPTIONS=("--build" "$BUILD_DIR" --config "$BUILD_TYPE")
if [ -n "$BUILD_VERBOSE_FLAG" ]; then
    BUILD_OPTIONS+=("$BUILD_VERBOSE_FLAG")
fi
cmake "${BUILD_OPTIONS[@]}"

echo ""
echo "Build complete: $BUILD_TYPE"
echo "Executable files are located in:"
echo "  $SCRIPT_DIR/$BUILD_DIR/bin/"
