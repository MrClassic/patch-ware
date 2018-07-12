/* **********************************************************************
 * File:   QuadraticWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      Implemented basic features
 ********************************************************************* */

#include "QuadraticWaveGenerator.h"

QuadraticWaveGenerator::QuadraticWaveGenerator() {
    frequency = 1.0;
    currentTime = 0.0;
    amplitude = 1.0;
    phase = 0.;
}

QuadraticWaveGenerator::QuadraticWaveGenerator(double frequency){
    this->frequency = frequency;
    currentTime = 0.0;
    amplitude = 1.0;
    phase = 0.;
}

QuadraticWaveGenerator::QuadraticWaveGenerator(const QuadraticWaveGenerator& orig) {
    frequency = orig.frequency;
    currentTime = orig.currentTime;
    amplitude = orig.amplitude;
    phase = orig.phase;
}

QuadraticWaveGenerator::~QuadraticWaveGenerator() {
    //do nothing...?
}

bool QuadraticWaveGenerator::pushDouble(){
    if(!paramsReady()){
        return false;
    }
    double signal = 2 * (double)amplitude * sqrt(frequency * getPhaseOffset()) - (double)amplitude;
    output(signal);
    return true;
}

