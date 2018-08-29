/* ************************************************************************
 * File:   Compressor.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/4/17
 *      File Created
 *		8/15/18
 *		implemented Para,eterizable interface
 *********************************************************************** */

#include "Compressor.h"
#include "Patch.h"
/* ***********************************************************************
 * 
 *                          Compressor Implementation
 * 
 *********************************************************************** */

/* ***********************************************************************
 *                              Constructors
 *********************************************************************** */
Compressor::Compressor() {
    params["max"] = 1.0;
    params["threshold"] = 1.0;
    params["buffer"] = 0.0;
    params["bypass"] = 0.0;
}

Compressor::Compressor(const Compressor& orig) {
	//copy over parameters
	copyParameters(orig);
}

/* ***********************************************************************
 *                              Destructor
 *********************************************************************** */
Compressor::~Compressor() {/* do nothing */}

/* ***********************************************************************
 *                              Accessors
 *********************************************************************** */

bool Compressor::process(){

    //inputs not ready, do nothing
    if(!*this)
        return false;
    if(params["buffer"].isPatched() && !params["buffer"].isReady()){
        return false;
    }
    if(params["threshold"].isPatched() && !params["threshold"].isReady()){
        return false;
    }
    if(params["max"].isPatched() && !params["max"].isReady()){
        return false;
    }

	//update parameters if they are patched
    params["buffer"].process();
    params["threshold"].process();
    params["max"].process();

    double signal = input();
    if(params["bypass"]){
		//bypassed? okay, just push the signal through
        output(signal);
		return true;
    }

	// *** calculate compression ***

    bool negative = signal < 0;
    double outSignal = 0.0;
    if(negative){
		//convert signal to positive for math
        signal *= -1;
    }
    if(params["buffer"] < signal){ //needs compression
		//calculate compression
        outSignal = ((signal - params["buffer"]) * (params["threshold"] - params["buffer"])) / (params["max"] - params["buffer"]) + params["buffer"];
    }
    else{ //signal under compression level
		//do not compress
        outSignal = signal;
    }
    if(negative){
		//convert back to negative
        outSignal *= -1;
    }

	//output
    output(outSignal * (params["max"]/ params["threshold"]));
    return true;
}

