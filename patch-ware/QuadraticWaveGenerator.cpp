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
 ********************************************************************* */

#include "QuadraticWaveGenerator.h"

QuadraticWaveGenerator::QuadraticWaveGenerator(){ /* Do Nothing */ }

QuadraticWaveGenerator::QuadraticWaveGenerator(double frequency){
	//set frequency
    params["frequency"] = frequency;
}

QuadraticWaveGenerator::QuadraticWaveGenerator(const QuadraticWaveGenerator& orig) {
	//copy stuff over to new Quadratic Wave Generator
	currentTime = orig.currentTime;
	copyParameters(orig);
}

QuadraticWaveGenerator::~QuadraticWaveGenerator() { /* do nothing...? */ }

bool QuadraticWaveGenerator::process(){

	//check parameters
    if(!*this){
        return false;
    }

	//output current wave value
    double signal = 2 * (double)params["amplitude"] * sqrt(params["frequency"] * getPhaseOffset()) - (double)params["amplitude"];
    output(signal);

	//success
    return true;
}

