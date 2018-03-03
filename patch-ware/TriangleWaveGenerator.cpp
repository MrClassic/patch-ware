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

bool TriangleWaveGenerator::pushDouble() const{
    if(!paramsReady()){
        return false;
    }
    //first spike incline
    if(currentTime < 1.0 / (frequency * 4.0) + (double)phase){
        output(amplitude * (currentTime * 4.0 * ((double)frequency) + (double)phase));
    }
    //second spike incline
    else if(currentTime > 3.0 / (frequency * 4.0) + (double)phase){
        output(amplitude * (currentTime * (frequency * 4.0) - 4.0 * (double)amplitude) + (double)phase);
    }
    //decline between spikes
    else{
        output(-1.0 * (double)amplitude * (currentTime * (frequency * 4.0) + 2.0 * (double)amplitude) + (double)phase);
    }
    return true;
}



