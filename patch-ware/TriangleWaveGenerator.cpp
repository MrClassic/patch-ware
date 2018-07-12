/* *****************************************************************
 * File:   TriangleWaveGenerator.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      4/30/17
 *      File Created
 *************************************************************** */

#include "TriangleWaveGenerator.h"

TriangleWaveGenerator::TriangleWaveGenerator() {
    frequency = 1.0;
    currentTime = 0.0;
    amplitude = 1.0;
}

TriangleWaveGenerator::TriangleWaveGenerator(const double freqency){
    this->frequency = frequency;
    currentTime = 0.0;
    amplitude = 1.0;
}

TriangleWaveGenerator::TriangleWaveGenerator(const TriangleWaveGenerator& orig) {
    frequency = orig.frequency;
    currentTime = orig.currentTime;
    amplitude = orig.amplitude;
}

TriangleWaveGenerator::~TriangleWaveGenerator() {
    //...do nothing?
}

bool TriangleWaveGenerator::pushDouble(){
    if(!paramsReady()){
        return false;
    }
    //first spike incline
    double offset = getPhaseOffset();
    if(offset < 1.0 / (frequency * 4.0)){
        output(offset * ((amplitude * 4.0) * ((double)frequency)));
    }
    //second spike incline
    else if(offset > 3.0 / (frequency * 4.0)){
        output(amplitude * offset * (frequency * 4.0) - 4.0 * (double)amplitude);
    }
    //decline between spikes
    else{
        output(-1.0 * (double)amplitude * (frequency * 4.0) * offset + 2.0 * (double)amplitude);
    }
    return true;
}



