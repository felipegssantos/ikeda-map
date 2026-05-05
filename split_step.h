#pragma once

#include <complex>
#include <valarray>
#include "config.h"

// Describes field A(t, z) for all t and fixed z;
typedef std::valarray<std::complex<double>> TimeField;

TimeField step (const TimeField&, const Config&);
