#include <complex>
#include <iostream>
#include <numeric>
#include <valarray>
#include <fftw3.h>
#include "split_step.h"

using std::complex;

// TODO: reuse fftw plans, maybe optimize plan if NLSE problem is big enough
void fourier (TimeField& field, int fftw_direction) {
    // Compute FFT in-place; direction given by fftw_direction

    // Create FFT plan
    fftw_plan plan;
    plan = fftw_plan_dft_1d(field.size(),
        reinterpret_cast<fftw_complex*>(&field[0]),
        reinterpret_cast<fftw_complex*>(&field[0]),
        fftw_direction, FFTW_ESTIMATE);
    // Compute FFT
    fftw_execute(plan);
    // Free memory
    fftw_destroy_plan(plan);
}

void fourier (TimeField& field) {
    // Compute forward FFT in-place
    fourier(field, FFTW_FORWARD);
}

void ifourier (TimeField& field) {
    // Compute inverse FFT in-place
    fourier(field, FFTW_BACKWARD);
}

TimeField step (const TimeField& field, const Config& config) {
    // TODO: use normalized form of NLSE instead
    const complex<double> I = {0, 1};
    // Take half a step with nonlinear part only
    const complex<double> prefactor = I * config.nonlinear_coupling * config.step_size / 2.0;
    TimeField out_field = exp(prefactor * pow(abs(field), 2)) * field;

    // Take full step with linear part only (requires FFTW)
    std::cout << "FFT" << std::endl;
    fourier(out_field);  // go to frequency domain

    // Build frequency vector
    auto size = field.size();
    complex<double> freq[size];
    std::iota(freq, freq + size, 0);
    std::valarray<complex<double>> omega(freq, size);
    omega *= 2 * M_PI;

    std::cout << "Linear propagation" << std::endl;
    out_field *= exp(0.5 * pow(omega, 2) * config.step_size * I);  // propagate in space
    std::cout << "Inverse FFT" << std::endl;
    // TODO: normalize after inverse FFT so energy is the same
    ifourier(out_field);  // go back to time domain

    // Take half a step with nonlinear part only, again
    std::cout << "Nonlinear step" << std::endl;
    out_field = exp(prefactor * pow(abs(out_field), 2)) * out_field;
    return out_field;
}
