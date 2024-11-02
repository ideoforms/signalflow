#!/bin/bash

#--------------------------------------------------------------------------------
# Build script for Raspberry Pi, which creates Linux/arm builds.
# Requires pyenv, which requires libffi-dev to build Python with ctypes support.
#
# May be ultimately better to switch to this arm runner action:
# https://github.com/pguyot/arm-runner-action
#--------------------------------------------------------------------------------

# Fail if any subcommands fail
set -e 

curl https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h -o source/include/signalflow/node/io/output/miniaudio-library.h

# Install dependencies to build a fully-fledged Python install
sudo apt-get install build-essential libffi-dev libreadline-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev

# Install dependencies to build a manylinux wheel
sudo apt-get patchelf

VERSIONS="3.8.20 3.9.20 3.10.15 3.11.10 3.12.7 3.13.0"

pyenv install --skip-existing $VERSIONS

for VERSION in $VERSIONS
do
    echo "Building version: $VERSION"
    pyenv local $VERSION
    python3 --version
    python3 -m build --wheel
done

pip3 install build auditwheel
auditwheel repair --plat manylinux_2_34_aarch64 dist/*.whl
