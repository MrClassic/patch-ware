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

	//set parameters to defaults
	addParameter("threshold");
    params["threshold"] = 1.;

}

Distortion::Distortion(const Distortion& orig) {
	//copy parameters
	copyParameters(orig);
}

Distortion::~Distortion() {/* Do nothing */}

bool Distortion::process(){

	//check for ready state
    if(!*this || !parametersReady()){
        return false;
    }

	//update parameters
	updateParameters();

	//get input signal
    double signal = input();
	if (params["bypass"]) {
		output(signal);
	}
	double thresh = params["threshold"];

	//run distortion algorithm
    if(signal > 0. && pw_abs(thresh) < signal){
        output(thresh);
    }
    else if(signal < 0. && pw_abs(thresh) < signal * -1.){
        output((thresh * -1.));
    }
    else{
        output(signal);
    }

	//success!
    return true;
}

