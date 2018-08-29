/* *****************************************************************
 * File:   TriangleWaveGenerator.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      4/30/17
 *      File Created
 *		8/16/18
 *		Happy Birthday Wifey!!!
 *		implemented Parameterizable interface
 *************************************************************** */

#include "TriangleWaveGenerator.h"

TriangleWaveGenerator::TriangleWaveGenerator() {
}

TriangleWaveGenerator::TriangleWaveGenerator(const double frequency){
    params["frequency"] = frequency;
}

TriangleWaveGenerator::TriangleWaveGenerator(const TriangleWaveGenerator& orig) {
	copyParameters(orig);
    currentTime = orig.currentTime;
}

TriangleWaveGenerator::~TriangleWaveGenerator() {
    //...do nothing?
}

bool TriangleWaveGenerator::process(){
    if(!*this){
        return false;
    }
    
	//get offset
    double offset = getPhaseOffset();

	//first spike incline
    if(offset < 1.0 / (params["frequency"] * 4.0)){
        output(offset * ((params["amplitude"] * 4.0) * ((double)params["frequency"])));
    }
    //second spike incline
    else if(offset > 3.0 / (params["frequency"] * 4.0)){
        output(params["amplitude"] * offset * (params["frequency"] * 4.0) - 4.0 * (double)params["amplitude"]);
    }
    //decline between spikes
    else{
        output(-1.0 * (double)params["amplitude"] * (params["frequency"] * 4.0) * offset + 2.0 * (double)params["amplitude"]);
    }
    return true;
}



