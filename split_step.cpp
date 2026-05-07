#include <complex>
#include <iostream>
#include <numeric>
#include <valarray>
#include <fftw3.h>
#include <vector>
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

TimeField step (const TimeField& field, const Config& config, double ts) {
    // TODO: use normalized form of NLSE instead
    const complex<double> I = {0, 1};
    // Take half a step with nonlinear part only
    const complex<double> prefactor = I * config.nonlinear_coupling * config.step_size / 2.0;
    TimeField out_field = exp(prefactor * pow(abs(field), 2)) * field;

    // Take full step with linear part only (requires FFTW)
    fourier(out_field);  // go to frequency domain

    // Build frequency vector
    auto size = field.size();
    complex<double> freq[size];
    std::iota(freq, freq + size, 0);
    std::valarray<complex<double>> omega(freq, size);
    omega *= 2 * M_PI / ts;

    out_field *= exp(0.5 * pow(omega, 2) * config.step_size * I);  // propagate in space
    // TODO: normalize after inverse FFT so energy is the same
    ifourier(out_field);  // go back to time domain
    out_field /= out_field.size();

    // Take half a step with nonlinear part only, again
    out_field = exp(prefactor * pow(abs(out_field), 2)) * out_field;
    return out_field;
}

void write_output(const std::vector<TimeField>& field, const Config& config) {
    std::ofstream file(config.output_path);
    for (auto arr : field) { // arr is the field for fixed z, all t
        for (int i = 0; i != arr.size(); ++i) {
            // each line has the field for fixed z, running over t
            file << arr[i].real();
            if (arr[i].imag() < 0)
                file << "-" << -arr[i].imag() << "j";
            else
                file << "+" << arr[i].imag() << "j";
            if (i != arr.size() - 1)
                file << ",";
        }
        file << std::endl;
    }
}
