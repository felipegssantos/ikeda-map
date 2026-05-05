/*
 * Goal: simulate the Ikeda map for a single resonator
 *
 * The Ikeda map is the dynamics of several fields $A^m(z, t)$, all obeying the
 * same nonlinear Schrodinger equation, and related to each by the boundary
 * conditions:
 *
 * $$
 * A^{m+1}(0, t) = \sqrt{\theta} A_{in} + \sqrt{1 - \theta} e^{i \delta_0} A^m(0, t)
 * $$
 *
 * The fields $A^m$ are complex functions while $z \in [0, L]$ and $t \in [0,
 * \infty)$.
 */


// TODO: get either config_path from input, e.g. "./simulate <config_path>"
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>
#include "config.h"
#include "split_step.h"

using std::ifstream, std::cout, std::endl, std::vector;

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
    vector<TimeField> field;  // TODO: initialize field A(t, 0)
    field.push_back(TimeField(1e-4, 16));
    // TODO: check config.length / config.step_size is integer
    while (z <= config.length) {
        cout << "Running for z = " << z << endl;
        field.push_back(step(field.back(), config));
        z += config.step_size;
    }
    // Run simulation
    // 1. Implement split-step method to solve NLSE
    // 2. Implement Ikeda map having a solver for the PDE

    // Save results
    return 0;
}
