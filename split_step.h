#pragma once

#include <complex>
#include <valarray>
#include <vector>
#include "config.h"

// Describes field A(t, z) for all t and fixed z;
typedef std::valarray<std::complex<double>> TimeField;

TimeField step (const TimeField&, const Config&, double);

void write_output(const std::vector<TimeField>&, const Config&);
