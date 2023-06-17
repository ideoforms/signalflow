#!/bin/bash

# fail on any bad exits
set -e 

pyenv install 3.8.17 3.9.17 3.10.12 3.11.4

for VERSION in 3.8.17 3.9.17 3.10.12 3.11.4
do
    pyenv local $VERSION
    python3 --version
    pip3 install build
    python3 -m build --wheel
done

echo "Renaming to _armv7l..."
SIGNALFLOW_VERSION=$(cat setup.py | grep version= | tr -dc '0-9.')
echo "SignalFlow version = $SIGNALFLOW_VERSION"
cd dist
mkdir $SIGNALFLOW_VERSION
for FILE in signalflow-${SIGNALFLOW_VERSION}*_aarch64.whl
do
    OUTPUT_FILE=$SIGNALFLOW_VERSION/$(echo $FILE | sed 's/aarch64/armv7l/')
    mv $FILE $OUTPUT_FILE
    echo "Created product: dist/$OUTPUT_FILE"
done

