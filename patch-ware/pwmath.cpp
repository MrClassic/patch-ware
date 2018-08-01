
#include "pwmath.h"

double pw_abs(double input) {
	if (input >= 0) {
		return input;
	}
	else {
		return input * -1.;
	}
}