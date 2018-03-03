/* ************************************************************************
 * File:   Compressor.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/4/17
 *      File Created
 *********************************************************************** */

#include "Compressor.h"
#include "Patch.h"
/* ***********************************************************************
 * 
 *                          Compressor Implementation
 * 
 *********************************************************************** */

/* ***********************************************************************
 *                              Constructors
 *********************************************************************** */
Compressor::Compressor() {
    max = 1.0;
    threshold = 1.0;
    buffer = 0.0;
    bypass = 0.0;
}

Compressor::Compressor(const Compressor& orig) {
    max = orig.max;
    threshold = orig.threshold;
    buffer = orig.buffer;
    bypass = orig.bypass;
}

/* ***********************************************************************
 *                              Destructor
 *********************************************************************** */
Compressor::~Compressor() {/* do nothing */}

/* ***********************************************************************
 *                              Accessors
 *********************************************************************** */

bool Compressor::process(){
    //inputs not ready, do nothing
    if(!*this)
        return false;
    if(buffer.getInputCount() > 0 && !buffer.isReady()){
        return false;
    }
    if(threshold.getInputCount() > 0 && !threshold.isReady()){
        return false;
    }
    if(max.getInputCount() > 0 && !max.isReady()){
        return false;
    }
    buffer.setParameter(buffer);
    threshold.setParameter(threshold);
    max.setParameter(max);
    double signal = averageInputs();
    if(bypass){
        output(signal);
    }
    bool negative = signal < 0;
    double outSignal = 0.0;
    if(negative){
        signal *= -1;
    }
    if(buffer < signal){ //needs compression
        outSignal = ((signal - buffer) * (threshold - buffer)) / (max - buffer) + buffer;
    }
    else{ //signal under compression level
        outSignal = signal;
    }
    if(negative){
        outSignal *= -1;
    }
    output(outSignal * (max / threshold));
    return true;
}

Parameter& Compressor::getThreshold(){
    return threshold;
}

Parameter& Compressor::getBuffer(){
    return buffer;
}

Parameter& Compressor::getMax(){
    return max;
}

/* ***********************************************************************
 *                              Mutators
 *********************************************************************** */

void Compressor::setThreshold(double threshold){
    this->threshold = threshold;
}

void Compressor::setBuffer(double buffer){
    this->buffer = buffer;
}

void Compressor::setMax(double max){
    this->max = max;
}
