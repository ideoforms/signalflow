#!/bin/bash

#------------------------------------------------------------------------
# generate-docs.sh
#
# Generates Signal documentation from Doxygen-format code comments.
#------------------------------------------------------------------------

if [ ! $(which doxygen) ]
then
	echo "Please install Doxygen to generate documentation."
	exit 1
fi

GIT_REPO_DIR=$(git rev-parse --show-toplevel)
cd "$GIT_REPO_DIR"

doxygen auxiliary/docs/signalflow.dox
