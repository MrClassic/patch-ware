/* **********************************************************************
 * File:   SquareWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/26/17
 *      File Created
 *      Implemented basic features
 *		7/9/18
 *		Adjusted push double method to support changes in amplitude
 *			- two solutions included. see process()
 *		8/16/18
 *		Happy Birthday Wifey!!!
 *		implemented Parameterizable interface
 *		10/10/18
 *		changed to implement the WaveProcessor interface
 ********************************************************************* */
#include "SquareWaveGenerator.h"

SquareWaveGenerator::SquareWaveGenerator() {
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::SquareWaveGenerator(double frequency) : SquareWaveGenerator() {
    params[FREQUENCY] = frequency;
}

SquareWaveGenerator::SquareWaveGenerator(const SquareWaveGenerator& orig) {
    waveOutput = orig.waveOutput;
    firstHalf = orig.firstHalf;
}

SquareWaveGenerator::~SquareWaveGenerator() {
    //do nothing...?
}

void SquareWaveGenerator::updateWaveOffset() {
    phaseCorrector = currentTime *
            (lastFreq - params[FREQUENCY]) + phaseCorrector;
}

/* ************************************************************************
* Push Double: Creates the next value in the square wave and sends it to
* the output channels. Includes two different algorithms which will
* eventually be split into two different wave generators.
*		- True square wave algorithm
*		- Malleable square wave algorithm
* 
* Pre-conditions: This square wave generator should be ready to accept
* any input from it's parameters
* Post-conditions: The next value of the wave being generated will be output
* to the next (set of) patch(es).
************************************************************************** */
double SquareWaveGenerator::generate() {
    
	//input into the wave function
    double in = currentTime * params[FREQUENCY] + getPhaseOffset();

    while (in < 0) {
        in += 1.; //********************
    }
    while (in > 1) {
        in -= 1; //********************
    }

    //malleable square wave equation
    //outputs raw amplitude value.
    //amplitude input will shape the wave
    /*
    if( in <  (0.5)) {
        waveOutput = params[AMPLITUDE];
    }
    else{
        waveOutput = -1 * params[AMPLITUDE];
    }
    */

    //True square wave algorithm
    //nature of the square wave is preserved
    //through fluxuating amplitude values.
    
    if (in < 0.5 && !firstHalf) {
        firstHalf = true;
        waveOutput = params[AMPLITUDE];
    }
    if(in >= 0.5 && firstHalf){
        firstHalf = false;
        waveOutput = params[AMPLITUDE] * -1.;
    }
    
	//output
    return waveOutput;
    
}

//EOF