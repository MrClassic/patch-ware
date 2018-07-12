/* **********************************************************************
 * File:   Gain.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/11/17
 *      File Created
 ********************************************************************* */

#include "Gain.h"
#include "Patch.h"

Gain::Gain() {
    bypass = false;
    level = 1.0;
}

Gain::Gain(const Gain& orig) {
    bypass = orig.bypass;
    level = orig.level;
}

Gain::~Gain() {
    //Do nothing =o)
}

Parameter& Gain::getLevel(){
    return level;
}

void Gain::setLevel(double level){
    this->level = level;
}

bool Gain::process(){
    if(!*this){
        return false;
    }
    if(level.getInputCount() > 0 && !level.isReady()){
        return false;
    }
    double signal = input();
    level.setParameter(level);
    if(!bypass){
        output(level * signal);
    }else{
        output(signal);
    }
    return true;
}