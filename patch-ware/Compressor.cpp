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

/* ***********************************************************************
 * 
 *                          Compressor Implementation
 * 
 *********************************************************************** */

/* ***********************************************************************
 *                              Constructors
 *********************************************************************** */
Compressor::Compressor() : SignalProcessor() {
	params.resize(NUM_PARAMS);
	params[BYPASS] = 0.0;
    params[MAX] = 1.0;
    params[THRESH] = 1.0;
    params[BUFFER] = 0.0;
}

Compressor::Compressor(const Compressor& orig) : SignalProcessor(orig){
	//do nothing...?
}

/* ***********************************************************************
 *                              Destructor
 *********************************************************************** */
Compressor::~Compressor() {/* do nothing */}

/* ***********************************************************************
 *                              Accessors
 *********************************************************************** */

double Compressor::processSignal(const double &signal){

	if (params[BYPASS] >= 1.)
		return signal;

	// *** calculate compression ***
	//BUFFER is where compression starts happening
	//THRESH is where compression maxes out before being boosted
	//MAX is the final signal's max output
    bool negative = signal < 0;

	double sig = signal;
    double outSignal = 0.0;
    if(negative){
		//convert signal to positive for math
        sig *= -1;
    }
    if(params[BUFFER] < sig){ //needs compression
		//calculate compression
        outSignal = ((sig - params[BUFFER]) * (params[THRESH] - params[BUFFER])) / (params[MAX] - params[BUFFER]) + params[BUFFER];
    }
    else{ //signal under compression level
		//do not compress
        outSignal = sig;
    }
    if(negative){
		//convert back to negative
        outSignal *= -1;
    }

	//output
    return (outSignal * (params[MAX]/ params[THRESH]));

}

