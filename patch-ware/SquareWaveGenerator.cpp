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
}

SquareWaveGenerator::SquareWaveGenerator(double frequency){
    this->frequency = frequency;
    currentTime = 0.0;
    amplitude = 1.0;
}

SquareWaveGenerator::SquareWaveGenerator(const SquareWaveGenerator& orig) {
    frequency = orig.frequency;
    currentTime = orig.currentTime;
    amplitude = orig.amplitude;
}

SquareWaveGenerator::~SquareWaveGenerator() {
    //do nothing...?
}

bool SquareWaveGenerator::pushDouble() const{
    if(!paramsReady()){
        return false;
    }
    if(currentTime < 1. / (frequency * 2.) + (double)phase){
        output((double)amplitude);
    }
    else{
        output(-1 * (double)amplitude);
    }
    return true;
}

//EOF