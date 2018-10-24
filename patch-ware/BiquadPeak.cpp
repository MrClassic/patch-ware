/* ******************************************************************
 * File:   BiquadBandpass.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      10/24/18
 *		Created
 ***************************************************************** */

#include "BiquadPeak.h"
#include "pwmath.h"
#include "sineTable.h"

BiquadPeak::BiquadPeak() : BiquadFilter() {

	params.resize(NUM_PARAMS);

}

BiquadPeak::BiquadPeak(const BiquadPeak& orig) : BiquadFilter(orig) {

	params[PEAK] = orig.params[PEAK];
	params[Q] = orig.params[Q];
	params[FC] = orig.params[FC];

}

BiquadPeak::~BiquadPeak() {

}

double BiquadPeak::processSignal(const double &signal) {

	updateCoefficients();

	return BiquadFilter::processSignal(signal);

}

void BiquadPeak::updateCoefficients() {
	if (lastQ != params[Q] || lastPeak != params[PEAK] || lastFC != params[FC]) {
		double v, k, norm;
		v = pow(10., pw_abs(params[PEAK]) / 20.);
		k = tan(PI * params[FC] / frameRate); //<------ frameRate or (1. / frameRate)???

		if (params[PEAK] >= 0) { //<--- frequency boost
			norm = 1. / (1. + 1. / params[Q] * k + k * k);

			params[A0] = (1. + v / params[Q] * k + k * k) * norm;
			params[A1] = 2 * (k * k - 1.) * norm;
			params[A2] = (1. - v / params[Q] * k + k * k) * norm;

			params[B0] = 1.0;
			params[B1] = params[A1];
			params[B2] = (1. - 1. / params[Q] * k + k * k) * norm;
		}
		else { //<--------------------- frequency cut
			norm = 1. / (1. + v / params[Q] * k + k * k);

			params[A0] = (1. + 1. / params[Q] * k + k * k) * norm;
			params[A1] = 2. * (k * k - 1.) * norm;
			params[A2] = (1. - 1. / params[Q] * k + k * k) * norm;

			params[B0] = 1.0;
			params[B1] = params[A1];
			params[B2] = (1. - v / params[Q] * k + k * k) * norm;
		}

		lastQ = params[Q];
		lastFC = params[FC];
		lastPeak = params[PEAK];
	}
}

//EOF