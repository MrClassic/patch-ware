/* *****************************************************************
 * File:   SawtoothWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      implementations for virtually inherited functions
 *		8/16/18
 *		Happy Birthday Wifey!!!
 *		implemented Parameterizable interface
 *
 *      TO DO:
 *      Add phase shift calculation for changing frequency
 ***************************************************************** */

#include "SawtoothWaveGenerator.h"

SawtoothWaveGenerator::SawtoothWaveGenerator() { /* Do Nothing*/ }

SawtoothWaveGenerator::SawtoothWaveGenerator(const double frequency){
	params["frequency"] = frequency;
}

SawtoothWaveGenerator::SawtoothWaveGenerator(const SawtoothWaveGenerator& orig) {
	copyParameters(orig);
	currentTime = orig.currentTime;
}

SawtoothWaveGenerator::~SawtoothWaveGenerator() { /* Do nothing */ }

bool SawtoothWaveGenerator::process(){
	//check parameters
    if(!*this)
        return false;

	//calculate and output
    double signal = params["amplitude"] * 2. * getPhaseOffset() / (1. / (double)params["frequency"]) - (double)params["amplitude"];
    output(signal);

	//success
    return true;
}
