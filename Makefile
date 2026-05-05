# TODO: move to a `build` directory

CXX = clang++

objects = config.o split_step.o main.o
fftw = /usr/local/lib/libfftw3.a

ikeda: config.o split_step.o main.o
	clang++ -o ikeda $(objects) $(fftw)

config.o: config.h
main.o split_step.o: config.h split_step.h

.PHONY: clean test-fft

clean:
	-rm ikeda $(objects)

test-fft: test_fourier.cpp
	clang++ test_fourier.cpp -o test_fourier $(fftw)
	./test_fourier
	rm test_fourier
