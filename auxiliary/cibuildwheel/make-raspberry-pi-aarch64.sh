#!/bin/bash

#--------------------------------------------------------------------------------
# Build script for Raspberry Pi, which creates Linux/arm builds.
# Tested on Raspberry Pi 4 running Raspberry Pi OS circa October 2024
# Requires pyenv, which requires libffi-dev to build Python with ctypes support.
#
# May be ultimately better to switch to this arm runner action:
# https://github.com/pguyot/arm-runner-action
#--------------------------------------------------------------------------------

# Fail if any subcommands fail
set -e 

# Install dependencies to build a fully-fledged Python install
sudo apt-get install build-essential libffi-dev libreadline-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev

# Dependencies for auditwheel, required to make manylinux build
sudo apt install patchelf
pip install auditwheel

VERSIONS="3.8.20 3.9.20 3.10.15 3.11.10 3.12.7 3.13.0"

pyenv install --skip-existing $VERSIONS

for VERSION in $VERSIONS
do
    pyenv local $VERSION
    python3 --version
    pip3 install build
    python3 -m build --wheel
done

cd dist
mkdir -p fixed
auditwheel repair *.whl -w fixed --plat manylinux_2_34_aarch64
