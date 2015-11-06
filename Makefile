INCLUDE_PATHS=/usr/local/include
LIBRARY_PATHS=/usr/local/lib
LIBRARIES=-l soundio -l sndfile

SOURCE_FILES=signum/*.cpp signum/op/*.cpp signum/io/*.cpp signum/gen/*.cpp signum/env/*.cpp
FLAGS=-std=c++1z -Wall

all:
	g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) signum-test.cpp -o signum-test
