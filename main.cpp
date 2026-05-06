/*
 * Goal: simulate the Ikeda map for a single resonator
 */

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

    // TODO: save results
    return 0;
}
