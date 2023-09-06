#ifndef COMPONENTS_COMPONENTS_UTILITIES_H
#define COMPONENTS_COMPONENTS_UTILITIES_H

#define components_constrain(value, low, high) ((value)<(low)?(low):((value)>(high)?(high):(value)))
#define components_round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define components_cube(x) ((x)*(x)*(x))
#define components_min(a, b) ((a)<(b)?(a):(b))
#define components_max(a, b) ((a)>(b)?(a):(b))

#include "fixed_trigonometry.h"

#endif
