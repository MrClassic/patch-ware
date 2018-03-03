/* *****************************************************************
 * File:   SawtoothWaveGenerator.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      implementations for virtually inherited functions
 *      TO DO:
 *      Add phase shift calculation for changing frequency
 ***************************************************************** */

#include "SawtoothWaveGenerator.h"

SawtoothWaveGenerator::SawtoothWaveGenerator() {
    frequency = 1.0;
    currentTime = 0.0;
    amplitude = 1.0;
}

SawtoothWaveGenerator::SawtoothWaveGenerator(const double frequency){
    this->frequency = frequency;
    currentTime = 0.0;
    amplitude = 1.0;
}

SawtoothWaveGenerator::SawtoothWaveGenerator(const SawtoothWaveGenerator& orig) {
    frequency = orig.frequency;
    currentTime = orig.currentTime;
    amplitude = orig.amplitude;
}

SawtoothWaveGenerator::~SawtoothWaveGenerator() {
    //do nothing...?
}

bool SawtoothWaveGenerator::pushDouble() const{
    if(!paramsReady())
        return false;
    double signal = amplitude * 2. * (currentTime / (1. / (double)frequency) + (double)phase) - (double)amplitude;
    output(signal);
    return true;
}
