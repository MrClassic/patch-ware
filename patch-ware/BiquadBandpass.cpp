/* ******************************************************************
 * File:   BiquadBandpass.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      10/24/18
 *		Created
 ***************************************************************** */

#include "BiquadBandpass.h"
#include "pwmath.h"
#include "sineTable.h"

BiquadBandpass::BiquadBandpass() : BiquadFilter() {

	params.resize(NUM_PARAMS);

}

BiquadBandpass::BiquadBandpass(const BiquadBandpass& orig) : BiquadFilter(orig) {
	params[PEAK] = orig.params[PEAK];
	params[Q] = orig.params[Q];
	params[FC] = orig.params[FC];

}

BiquadBandpass::~BiquadBandpass() {

}

double BiquadBandpass::processSignal(const double &signal){
	
	updateCoefficients();

	return BiquadFilter::processSignal(signal);
    
}

void BiquadBandpass::updateCoefficients() {
	if (lastQ != params[Q] || lastPeak != params[PEAK] || lastFC != params[FC]) {
		double v, k, norm;
		v = pow(10., pw_abs(params[PEAK]) / 20.);
		k = tan(PI * params[FC] / frameRate); //<------ frameRate or (1. / frameRate)???

		norm = 1. / (1. + k / params[Q] + k * k);
		params[A0] = k / params[Q] * norm;
		params[A1] = 0.;
		params[A2] = -1. * params[A0];

		params[B0] = 1.;
		params[B1] = 2 * (k * k - 1.) * norm;
		params[B2] = (1. - k / params[Q] + k * k) * norm;

		lastQ = params[Q];
		lastFC = params[FC];
		lastPeak = params[PEAK];
	}
}

//EOF