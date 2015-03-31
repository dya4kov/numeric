#pragma once
#include <math.h>
#include "numericTypes.h"

Int sign(Double x) {
	return x >= 0 ? 1 : -1;
}

Double abs(Double x) {
	return fabs(x);
}

Double min(Double x, Double y) {
	return x >= y ? y : x;
}

Double max(Double x, Double y) {
	return x >= y ? x : y;
}

Double sqr(Double x) {
	return x*x;
}