# TODO: move compiled files to a `build` directory

CXX = clang++
vpath %.cpp src
vpath %.h src
vpath %.a /usr/local/lib  # where to search for libfftw3.a

objects = config.o split_step.o main.o

ikeda: $(objects) -lfftw3
	clang++ -o $@ $^

config.o: config.h
main.o split_step.o: config.h split_step.h

.PHONY: clean test-fft

clean:
	-rm ikeda $(objects)

test-fft: test_fourier.cpp
	clang++ test_fourier.cpp -o test_fourier $(fftw)
	./test_fourier
	rm test_fourier
