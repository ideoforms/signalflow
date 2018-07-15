#!/bin/bash

#------------------------------------------------------------------------
# Generate README for Signal examples.
#------------------------------------------------------------------------

echo "# Signal Examples"
echo

for file in *.cpp
do
	echo "**[$file]($file)**  "
	cat $file | head -6 | grep [a-z] | sed -e 's/^[ *]*//' | tail +2 
	echo
done
