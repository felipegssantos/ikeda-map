/*
 * Goal: simulate the Ikeda map for a single resonator
 */

#include <iostream>
#include <vector>
#include "config.h"
#include "split_step.h"

using std::cout, std::endl, std::vector;

int main (int argc, char* argv[]) {
    // Check config file was provided
    if (argc == 1) {
        cout << "path argument required" << endl;
        return 1;
    }
    else if (argc > 2) {
        cout << "expected only one argument" << endl;
        return 1;
    }

    // Read a config file to set up simulation
    Config config = read(argv[1]);

    // Solve NLSE
    double z = 0;
    vector<TimeField> field;

    // Initialize field A(t, 0)
    // TODO: move initialization to config file
    int size = pow(2, 10);
    double ts = 1e-6 / size;  // sampling time
    double squared_width = pow(6e-8, 2); //pow(0.5, 2); // pulse width
    double tc = ts * size / 2;  // pulse center
    std::complex<double> init[size];
    for (int i = 0; i != size; ++i) {
        double t = i * ts;
        init[i] = exp(-pow(t - tc, 2) / (2 * squared_width));
    }

    field.push_back(TimeField(init, size));
    cout << "Field initialized" << endl;
    // TODO: check config.length / config.step_size is integer
    while (z <= config.length) {
        // cout << "Running for z = " << z << endl;
        field.push_back(step(field.back(), config, ts));
        z += config.step_size;
    }
    cout << "Split-step finished" << endl;

    // Save results
    write_output(field, config);
    cout << "Saved outputs to " << config.output_path << endl;
    return 0;
}
