INCLUDE_PATHS=/usr/local/include
LIBRARY_PATHS=/usr/local/lib
LIBRARIES=-lsoundio -lsndfile -lgsl

SOURCE_FILES=signum/*.cpp signum/*/*.cpp
FLAGS=-std=c++1z -Wall -g

all:
	# g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) sine-env-test.cpp -o sine-env-test
	g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) simpler-test.cpp -o simpler-test

lib:
	# g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) sine-env-test.cpp -o sine-env-test
	g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) -shared -o libsignum.so

sndfile-test:
	g++ -I/usr/local/include -L/usr/local/lib sndfile-test.cpp -lsndfile  -o sndfile-test

.PHONY : liblo-test
liblo-test:
	g++ -I/usr/local/include -L/usr/local/lib liblo-test.cpp -llo -o liblo-test

.PHONY : liblo-synth-test
liblo-synth-test:
	g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) -l lo liblo-synth-test.cpp -o liblo-synth-test

.PHONY : signum-ptr-test
signum-ptr-test:
	g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) signum-ptr-test.cpp -o signum-ptr-test

.PHONY : signum-buffer-test
signum-buffer-test:
	g++ -I $(INCLUDE_PATHS) -L $(LIBRARY_PATHS) $(LIBRARIES) $(SOURCE_FILES) $(FLAGS) signum-buffer-test.cpp -o signum-buffer-test

.PHONY : sine-env-test
sine-env-test:
	g++ -I /usr/local/include -I . -L /usr/local/lib -L . -lsignum $(FLAGS) sine-env-test.cpp -o sine-env-test

