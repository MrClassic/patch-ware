/* **********************************************************************
 * File:   Gain.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface
 ********************************************************************* */

#include "Gain.h"

Gain::Gain() : SignalProcessor() {
	params.resize(NUM_PARAMS);
}

Gain::Gain(const Gain& orig) : SignalProcessor(orig) {
	//do nothing...?
}

Gain::~Gain() {
    //Do nothing =o)
}

double Gain::processSignal(const double &signal){

	//calculate and output
    if(params[BYPASS] >= 1.){
		return signal;
    }else{
		return signal * params[LEVEL];
    }

}