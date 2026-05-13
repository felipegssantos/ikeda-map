#include <complex>
#include <fstream>
#include <map>
#include <stdexcept>
#include <string>
#include "config.h"

using std::ifstream, std::string;

Config read(char* path) {

    ifstream config_file(path);
    if (!config_file.good())
        throw std::invalid_argument("cannot read config file");
    string line;
    std::map<string, string> parsed_file;
    // Assuming each line in config file has form "key=value"
    // (no spaces!!!)
    while (std::getline(config_file, line)) {
        string::size_type i = line.find('=');
        string key = line.substr(0, i);
        if (parsed_file.count(key))
            throw std::invalid_argument("repeated parameters in config file");
        parsed_file[key] = line.substr(i+1, line.size());
    }

    // Right now, this ignores any extra parameter in the config file
    Config config;
    try {
        config.loss = stod(parsed_file.at("loss"));
        config.detuning = stod(parsed_file.at("detuning"));
        config.dispersion = stod(parsed_file.at("dispersion"));
        config.length = stod(parsed_file.at("length"));
        config.nonlinear_coupling = stod(parsed_file.at("nonlinear_coupling"));
        config.energy_coupling = stod(parsed_file.at("energy_coupling"));
        config.output_path = parsed_file.at("output_path");
        config.step_size = stod(parsed_file.at("step_size"));
        config.input_field = std::complex(
            stod(parsed_file.at("input_field_real")),
            stod(parsed_file.at("input_field_imag")));
    } catch (std::out_of_range) {
        throw std::domain_error("missing parameter");
    }

    return config;
}
