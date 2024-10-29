#!/bin/bash

#--------------------------------------------------------------------------------
# Build script for Raspberry Pi, which creates Linux/arm builds.
# Requires pyenv, which requires libffi-dev to build Python with ctypes support.
#
# May be ultimately better to switch to this arm runner action:
# https://github.com/pguyot/arm-runner-action
# Or at least cibuildwheel
#--------------------------------------------------------------------------------

# Fail if any subcommands fail
set -e 

pyenv install 3.8.17 3.9.17 3.10.12 3.11.4

for VERSION in 3.8.17 3.9.17 3.10.12 3.11.4
do
    pyenv local $VERSION
    python3 --version
    pip3 install build
    python3 -m build --wheel
done

SIGNALFLOW_VERSION=$(cat setup.py | grep version= | tr -dc '0-9.')
echo "SignalFlow version = $SIGNALFLOW_VERSION"
cd dist
mkdir $SIGNALFLOW_VERSION
for FILE in signalflow-${SIGNALFLOW_VERSION}*_aarch64.whl
do
    echo "Renaming to _armv7l..."
    OUTPUT_FILE=$SIGNALFLOW_VERSION/$(echo $FILE | sed 's/aarch64/armv7l/')
    mv $FILE $OUTPUT_FILE
    echo "Created product: dist/$OUTPUT_FILE"
done
