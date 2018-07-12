/* ************************************************************************
 * File:   Distortion.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/5/17
 *      File Created
 */

#include "Distortion.h"
#include "Patch.h"

/* ************************************************************************
 * 
 *                          Distortion Implementation
 * 
 ************************************************************************* */

Distortion::Distortion() {
    threshold = 1.0;
    bypass = false;
}

Distortion::Distortion(const Distortion& orig) {
    threshold = orig.threshold;
    bypass = orig.bypass;
}

Distortion::~Distortion() {/* Do nothing */}

Parameter& Distortion::getThreshold(){
    return threshold;
}

bool Distortion::process(){
    if(!*this || (threshold.getInputCount() > 0 && !threshold.isReady())){
        return false;
    }
    threshold.setParameter(threshold);
    double signal = input();
    if(signal > 0 && threshold < signal && !bypass){
        output(threshold);
    }
    else if(signal < 0 && signal < (threshold * -1.) && !bypass){
        output((threshold * -1.));
    }
    else{
        output(signal);
    }
    return true;
}

void Distortion::setThreshold(double threshold){
    this->threshold = threshold;
}
