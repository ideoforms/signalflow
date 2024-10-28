#!/bin/bash

#--------------------------------------------------------------------------------
# Builds arm64 and x86_64 wheels for macOS
# Requires virtualenvs in the current directory, named venv-38, venv-39, etc...
# Should be retired when GitHub Actions support arm64 build
#--------------------------------------------------------------------------------

ROOT=auxiliary/cibuildwheel

for VERSION in 38 39 310 312 313 313
do
    rm -r build
    export CIBW_BUILD="cp${VERSION}-*"
    export CIBW_BUILD_VERBOSITY=1

    . $ROOT/venv-$VERSION/bin/activate
    pip3 install cibuildwheel delocate

    # For some reason, Python 3.13 seems to do additional validation on delocate which
    # throws an exception when dependencies have a deployment target version set too high,
    # and many of the dependencies on my build machine have a target of macOS 13 (Ventura).
    # Need to verify whether the pre-3.13 builds are actually truly compatible with pre-Ventura!
    if [ "$VERSION" == "313" ]; then
        export MACOSX_DEPLOYMENT_TARGET=13.0
    fi

    #--------------------------------------------------------------------------------
    # Make x86.
    #--------------------------------------------------------------------------------
    export REPAIR_LIBRARY_PATH=/usr/local/lib
    export CIBW_ARCHS_MACOS="x86_64"
    export CMAKE_OSX_ARCHITECTURES=x86_64
    export CIBW_REPAIR_WHEEL_COMMAND_MACOS="DYLD_LIBRARY_PATH=$REPAIR_LIBRARY_PATH delocate-wheel -w {dest_dir} -v {wheel}"

    python3 -m cibuildwheel --output-dir wheelhouse --platform macos

    #--------------------------------------------------------------------------------
    # Make arm64
    #--------------------------------------------------------------------------------
    export REPAIR_LIBRARY_PATH=/opt/homebrew/lib
    export CIBW_ARCHS_MACOS="arm64"
    export CMAKE_OSX_ARCHITECTURES=arm64
    export CIBW_REPAIR_WHEEL_COMMAND_MACOS="DYLD_LIBRARY_PATH=$REPAIR_LIBRARY_PATH delocate-wheel -w {dest_dir} -v {wheel}"

    python3 -m cibuildwheel --output-dir wheelhouse --platform macos
done
