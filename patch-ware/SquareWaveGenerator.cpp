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
 ********************************************************************* */
#include "SquareWaveGenerator.h"

SquareWaveGenerator::SquareWaveGenerator() {
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::SquareWaveGenerator(double frequency) {
    params["frequency"] = frequency;
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::SquareWaveGenerator(const SquareWaveGenerator& orig) {
	copyParameters(orig);
    currentTime = orig.currentTime;
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::~SquareWaveGenerator() {
    //do nothing...?
}

void SquareWaveGenerator::updateWaveOffset() {
    phaseCorrecter = currentTime *
            (lastFrequency - (double) params["frequency"]) + phaseCorrecter;
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
bool SquareWaveGenerator::process() {
    if (!paramsReady()) {
        return false;
    }
    /*
	!!!! Debug line !!!!
    if(currentTime > 0.5){
        int stall = 0;
    }
     * */
    setFrequency((double) params["frequency"]);
    setAmplitude((double) params["amplitude"]);
    setPhase((double) params["phase"]);

	//input into the wave function
    double in = currentTime * params["frequency"] + getPhaseOffset();

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
        waveOutput = (double)params["amplitude"];
    }
    else{
        waveOutput = -1 * (double)params["amplitude"];
    }
    */

    //True square wave algorithm
    //nature of the square wave is preserved
    //through fluxuating amplitude values.
    
    if (in < 0.5 && !firstHalf) {
        firstHalf = true;
        waveOutput = (double)params["amplitude"];
    }
    if(in >= 0.5 && firstHalf){
        firstHalf = false;
        waveOutput = (double)params["amplitude"] * -1.;
    }
    
	//output
    output(waveOutput);
    
	//success
    return true;
}

//EOF