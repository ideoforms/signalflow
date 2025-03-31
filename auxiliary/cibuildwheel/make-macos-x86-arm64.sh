#!/bin/bash

#--------------------------------------------------------------------------------
# Builds arm64 and x86_64 wheels for macOS
#
# Requires virtualenvs in the current directory, named venv-38, venv-39, etc...
# Should ideally be retired in favour of GitHub Actions at some point
#
# Some notes on macOS compatibility versions:
#  - Unless an old MACOSX_DEPLOYMENT_TARGET is set, libraries compiled with a
#    newer macOS (e.g. libsndfile and its dependencies) are theoretically not
#    compatible with older macOS, and so this wheel must be built on a machine
#    with older compatibilities of libraries installed
#  - This is verified explicitly by newer versions of `delocate`, specifically
#    those used by Python 3.13+. For this, we set a minimum version of
#    macOS 12 (Monterey).
#  - But oddly, my tests show that some libraries (e.g. libsndfile 2.37.0 with
#    minOS of 13) do actually work OK on older macOS. No idea why. But we
#    exploit this by not setting a MACOSX_DEPLOYMENT_TARGET on older Python
#    builds, which falls back on 10.15 (Catalina)
#  - 10.15 is also used as a default target for Intel libraries
#--------------------------------------------------------------------------------

ROOT=auxiliary/cibuildwheel
set -e

for VERSION in 38 39 310 312 313
do
    rm -rf build
    export CIBW_BUILD="cp${VERSION}-*"
    export CIBW_BUILD_VERBOSITY=1

    . $ROOT/venv-$VERSION/bin/activate
    pip3 install cibuildwheel delocate


    #--------------------------------------------------------------------------------
    # Make x86.
    #--------------------------------------------------------------------------------
    export MACOSX_DEPLOYMENT_TARGET=10.15
    export REPAIR_LIBRARY_PATH=/usr/local/lib
    export CIBW_ARCHS_MACOS="x86_64"
    export CMAKE_OSX_ARCHITECTURES=x86_64
    export CIBW_REPAIR_WHEEL_COMMAND_MACOS="DYLD_LIBRARY_PATH=$REPAIR_LIBRARY_PATH delocate-wheel -w {dest_dir} -v {wheel}"

    python3 -m cibuildwheel --output-dir wheelhouse --platform macos

    # Check that wheel has been successfully linked against dependencies,
    # and that they are delocated. Failure to link can happen silently
    # if dependencies are not found!
    WHEEL_FILE=$(ls -tr wheelhouse/*.whl | tail -1)
    delocate-listdeps $WHEEL_FILE | grep signalflow-stubs/.dylibs/libsndfile >/dev/null

    #--------------------------------------------------------------------------------
    # Make arm64
    #--------------------------------------------------------------------------------
    # For Python 3.13 upwards (which use stricter delocate)
    # See notes above
    if [ "$VERSION" == "313" ]; then
        export MACOSX_DEPLOYMENT_TARGET=12.0
    else
        export MACOSX_DEPLOYMENT_TARGET=11.0
    fi

    export REPAIR_LIBRARY_PATH=/opt/homebrew/lib
    export CIBW_ARCHS_MACOS="arm64"
    export CMAKE_OSX_ARCHITECTURES=arm64
    export CIBW_REPAIR_WHEEL_COMMAND_MACOS="DYLD_LIBRARY_PATH=$REPAIR_LIBRARY_PATH delocate-wheel -w {dest_dir} -v {wheel}"

    python3 -m cibuildwheel --output-dir wheelhouse --platform macos
    WHEEL_FILE=$(ls -tr wheelhouse/*.whl | tail -1)
    delocate-listdeps $WHEEL_FILE | grep signalflow-stubs/.dylibs/libsndfile >/dev/null
done
