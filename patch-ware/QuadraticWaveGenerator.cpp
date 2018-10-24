/* **********************************************************************
 * File:   QuadraticWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      Implemented basic features
 *		8/16/18
 *		Happy Birthday Wifey!!!
 *		implemented Parameterizable interface
 *		10/10/18
 *		Changed to implement the WaveProcessor interface
 ********************************************************************* */

#include "QuadraticWaveGenerator.h"

QuadraticWaveGenerator::QuadraticWaveGenerator(){ /* Do Nothing */ }

QuadraticWaveGenerator::QuadraticWaveGenerator(double frequency){
	//set frequency
    params[FREQUENCY] = frequency;
}

QuadraticWaveGenerator::QuadraticWaveGenerator(const QuadraticWaveGenerator& orig) {
	//do nothing
}

QuadraticWaveGenerator::~QuadraticWaveGenerator() { /* do nothing...? */ }

void QuadraticWaveGenerator::updateWaveOffset() {
	//TODO: calculate wave offset for quadractic waves
}

double QuadraticWaveGenerator::generate(){

	//return current wave value
    return 2. * params[AMPLITUDE] * sqrt(params[FREQUENCY] * getPhaseOffset()) - params[AMPLITUDE];
    
}

