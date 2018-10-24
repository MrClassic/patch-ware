/* ************************************************************************
 * File:   Distortion.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/5/17
 *      File Created
		9/2/18
		Optimized the process() method:
			reduced map lookups
 */

#include "Distortion.h"

/* ************************************************************************
 * 
 *                          Distortion Implementation
 * 
 ************************************************************************* */

Distortion::Distortion() {

	params.resize(NUM_PARAMS);

	//set parameters to defaults
    params[THRESH] = 1.;

}

Distortion::Distortion(const Distortion& orig) : SignalProcessor(orig) {
	//do nothing...?
}

Distortion::~Distortion() {/* Do nothing */}

double Distortion::processSignal(const double &signal){

	if (params[BYPASS] >= 1.) {
		return signal;
	}

	//run distortion algorithm
    if(signal > 0. && pw_abs(params[THRESH]) < signal){
        return params[THRESH];
    }
    else if(signal < 0. && pw_abs(params[THRESH]) < signal * -1.){
        return params[THRESH] * -1.;
    }
    else{
        return signal;
    }

}

