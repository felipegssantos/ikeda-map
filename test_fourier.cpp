#include <complex>
#include <cstring>
#include <iostream>
#include <valarray>
#include <fftw3.h>
#include "split_step.h"

using std::cout, std::endl, std::valarray;

void fourier (TimeField& field) {
    // Compute forward FFT in-place
    fftw_plan plan;
    size_t N = field.size();
    TimeField result(N);
    plan = fftw_plan_dft_1d(
        N, reinterpret_cast<fftw_complex*>(&field[0]),
        reinterpret_cast<fftw_complex*>(&field[0]),
        FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

int main () {
    int size = 16;
    TimeField time(size);

    // Initialize time and signal arrays
    double omega = 2 * M_PI * 1;
    double ts = 0.5;
    for (double i = 0; i != size; ++i)
        time[i] = i * ts;

    TimeField signal = cos(time * omega);

    cout << "Input signal:" << endl;
    for (int i = 0; i != size; ++i)
        cout << time[i].real() << "\t" << signal[i] << endl;

    // Take Fourier transform
    fourier(signal);

    cout << endl << "FFT:" << endl;
    for (double i = 0; i != size; ++i)
        cout << i / ts << "\t" << signal[i] << endl;
}
