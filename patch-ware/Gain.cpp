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

Gain::Gain() {
    params["bypass"] = false;
    params["level"] = 1.0;
}

Gain::Gain(const Gain& orig) {
	copyParameters(orig);
}

Gain::~Gain() {
    //Do nothing =o)
}

bool Gain::process(){

	//check for ready state
	
    if(!isReady()){
        return false;
    }
    if(!parametersReady()){
        return false;
    }
	
	//get input
    double signal = input();

	//update parameter
	updateParameters();

	//calculate and output
    if(!params["bypass"]){
        output(params["level"] * signal);
    }else{
        output(signal);
    }

	//success!
    return true;
}