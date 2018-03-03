/* **********************************************************************
 * File:   SineWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      Implemented basic features
 ********************************************************************* */

#include "SineWaveGenerator.h"

SineWaveGenerator::SineWaveGenerator() {
    frequency = 1.0;
    currentTime = 0.0;
    amplitude = 1.0;
}

SineWaveGenerator::SineWaveGenerator(double frequency){
    this->frequency = frequency;
    currentTime = 0.0;
    amplitude = 1.0;
}

SineWaveGenerator::SineWaveGenerator(const SineWaveGenerator& orig) {
    frequency = orig.frequency;
    currentTime = orig.currentTime;
    amplitude = orig.amplitude;
}

SineWaveGenerator::~SineWaveGenerator() {
    //do nothing...?
}

bool SineWaveGenerator::pushDouble() const{
    if(!paramsReady()){
        return false;
    }
    double in = (double)frequency * currentTime * 360.0 + (double)phase;
    output(amplitude * sineD(in));
    return true;
}


