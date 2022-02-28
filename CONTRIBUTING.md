# Contributing

## Creating a new Node

- Create the .h header file in the appropriate subdirectory of `source/include/signal/node`
- Create the .cpp source file in the appropriate subdirectory of `source/src/node`
- Add the header file to `source/include/signal/signal.h`
- Add the source file to `source/src/CMakeLists.txt`
- Regenerate the auto-generated Python bindings: `auxiliary/scripts/generate-node-python-bindings.py > source/src/python/nodes.cpp`
- Add unit tests to the appropriate test script in `tests`
- Re-run the tests: `python3 setup.py test`

## Build: C++

To build with debug symbols:
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

## Build: Python

To build the Python libraries with debug symbols:
```
python3 setup.py build --debug install
# TODO: How to do this with pip?
```

## Test

To run the unit test suite:
```
python3 setup.py test
```

## Documentation

To generate and serve the docs:

```
pip3 install mkdocs mkdocs-material
mkdocs serve
```

To deploy docs to GitHub:
```
mkdocs gh-deploy
```
