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
 *		10/10/18
 *		changed to implement the WaveProcessor interface
 *************************************************************** */

#include "TriangleWaveGenerator.h"

TriangleWaveGenerator::TriangleWaveGenerator() {
}

TriangleWaveGenerator::TriangleWaveGenerator(const double frequency){
    params[FREQUENCY] = frequency;
}

TriangleWaveGenerator::TriangleWaveGenerator(const TriangleWaveGenerator& orig) {
	//do nothing
}

TriangleWaveGenerator::~TriangleWaveGenerator() {
    //...do nothing?
}

void TriangleWaveGenerator::updateWaveOffset() {
	//TODO: calculate and implement
}

double TriangleWaveGenerator::generate(){
    
	//get offset
    double offset = getPhaseOffset();

	/*

	|
	|----| first spike
	|    |
	|    |------| decline between spikes
	|    |      |
	|	 |      |----| Second spike
	|-----------------
	|   /\
	|  /  \
	| /    \
	|/      \        
	|        \      /
	|         \    /
	|          \  /
	|           \/
	|-----------------
	*/

	//first spike incline
    if(offset < 1.0 / (params[FREQUENCY] * 4.0)){
        return offset * ((params[AMPLITUDE] * 4.0) * params[FREQUENCY]);
    }
    //second spike incline
    else if(offset > 3.0 / (params[FREQUENCY] * 4.0)){
        return params[AMPLITUDE] * offset * (params[FREQUENCY] * 4.0) - 4.0 * params[AMPLITUDE];
    }
    //decline between spikes
    else{
        return -1.0 * params[AMPLITUDE] * (params[FREQUENCY] * 4.0) * offset + 2.0 * params[AMPLITUDE];
    }
    return true;
}



