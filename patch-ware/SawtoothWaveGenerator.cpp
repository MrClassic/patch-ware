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
 *		(don't worry, she's out of the house, i'm not a workaholic)
 *
 *      TO DO:
 *      Add phase shift calculation for changing frequency
 ***************************************************************** */

#include "SawtoothWaveGenerator.h"

SawtoothWaveGenerator::SawtoothWaveGenerator() { /* Do Nothing*/ }

SawtoothWaveGenerator::SawtoothWaveGenerator(const double frequency){
	params[FREQUENCY] = frequency;
}

SawtoothWaveGenerator::SawtoothWaveGenerator(const SawtoothWaveGenerator& orig) { /* Do Nothing */ }

SawtoothWaveGenerator::~SawtoothWaveGenerator() { /* Do nothing */ }

void SawtoothWaveGenerator::updateWaveOffset() {
	phaseCorrector = currentTime *
		(lastFreq - params[FREQUENCY]) + phaseCorrector;
}

double SawtoothWaveGenerator::generate(){
	
	//calculate and output
    return params[AMPLITUDE] * 2. * getPhaseOffset() / (1. / params[FREQUENCY]) - params[AMPLITUDE];
    
}
