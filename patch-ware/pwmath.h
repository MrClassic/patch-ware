#ifndef PWMATH_H
#define PWMATH_H

double pw_abs(double input) {
	if (input >= 0) {
		return input;
	}
	else {
		return input * -1.;
	}
}

#endif
