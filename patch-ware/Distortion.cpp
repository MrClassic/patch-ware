/* ************************************************************************
 * File:   Distortion.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/5/17
 *      File Created
 */

#include "Distortion.h"

/* ************************************************************************
 * 
 *                          Distortion Implementation
 * 
 ************************************************************************* */

Distortion::Distortion() {

	//set parameters to defaults
    params["threshold"] = 1.0;
    params["bypass"] = false;

}

Distortion::Distortion(const Distortion& orig) {
	//copy parameters
	copyParameters(orig);
}

Distortion::~Distortion() {/* Do nothing */}

bool Distortion::process(){

	//check for ready state
    if(!*this || (params["threshold"].isPatched() && !params["threshold"].isReady())){
        return false;
    }

	//update threshold parameter
    params["threshold"].process();

	//get input signal
    double signal = input();

	//run distortion algorithm
    if(signal > 0 && params["threshold"] < signal && !params["bypass"]){
        output(params["threshold"]);
    }
    else if(signal < 0 && signal < (params["threshold"] * -1.) && !params["bypass"]){
        output((params["threshold"] * -1.));
    }
    else{
        output(signal);
    }

	//success!
    return true;
}

