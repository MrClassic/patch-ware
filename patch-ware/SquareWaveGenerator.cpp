/* **********************************************************************
 * File:   SquareWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/26/17
 *      File Created
 *      Implemented basic features
 ********************************************************************* */
#include "SquareWaveGenerator.h"

SquareWaveGenerator::SquareWaveGenerator() {
    frequency = 1.0;
    currentTime = 0.0;
    amplitude = 1.0;
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::SquareWaveGenerator(double frequency) {
    this->frequency = frequency;
    currentTime = 0.0;
    amplitude = 1.0;
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::SquareWaveGenerator(const SquareWaveGenerator& orig) {
    frequency = orig.frequency;
    currentTime = orig.currentTime;
    amplitude = orig.amplitude;
    waveOutput = 0.;
    firstHalf = false;
}

SquareWaveGenerator::~SquareWaveGenerator() {
    //do nothing...?
}

void SquareWaveGenerator::updateWaveOffset() {
    phaseCorrecter = currentTime *
            (lastFrequency - (double) frequency) + phaseCorrecter;
}

bool SquareWaveGenerator::pushDouble() {
    if (!paramsReady()) {
        return false;
    }
    /*
    if(currentTime > 0.5){
        int stall = 0;
    }
     * */
    setFrequency((double) frequency);
    setAmplitude((double) amplitude);
    setPhase((double) phase);

    double in = currentTime * frequency + getPhaseOffset();

    while (in < 0) {
        in += 1.; //********************
    }
    while (in > 1) {
        in -= 1; //********************
    }

    //Fluxuating amplitude algorithm 
    //outputs raw amplitude value.
    //amplitude input will shape the wave
    
    if( in <  (0.5)) {
        waveOutput = (double)amplitude;
    }
    else{
        waveOutput = -1 * (double)amplitude;
    }
     

    //True square wave algorithm
    //nature of the square wave is preserved
    //through fluxuating amplitude values.
    /*
    if (in < 0.5 && !firstHalf) {
        firstHalf = true;
        waveOutput = (double)amplitude;
    }
    if(in >= 0.5 && firstHalf){
        firstHalf = false;
        waveOutput = (double)amplitude * -1.;
    }
    */
    output(waveOutput);
    
    return true;
}

//EOF