#pragma once

#include <complex>
#include <filesystem>
#include <fstream>

struct Config {
    double loss, detuning, dispersion;
    double length, nonlinear_coupling, energy_coupling;
    double step_size;
    std::complex<double> input_field;
    std::filesystem::path output_path;
};

Config read(char* path);
