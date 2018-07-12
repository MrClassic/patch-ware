/* ************************************************************************
 * File:   Delay.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/5/17
 *      File Created
 *      Implemented core functionality
 *********************************************************************** */

#include "Delay.h"
#include "Patch.h"

Delay::Delay() {
    decay = 0.5;
    registerSize = 0;
    circular_queue<double> temp(1, 0.);
    registers = temp;
    bypass = false;
}

Delay::Delay(const Delay& orig) {
    decay = orig.decay;
    bypass = orig.bypass;
    registers = orig.registers;
}

Delay::~Delay() {/* Do Nothing */}

Parameter& Delay::getDecay(){
    return decay;
}

bool Delay::process(){
    if(!*this || (decay.getInputCount() > 0 && !decay.isReady()))
        return false;
    decay.setParameter(decay);
    double signal = input();
    double outSignal = signal;
    outSignal += decay * registers.pop();
    registers.push(outSignal);
    output(outSignal);
    return true;
}

void Delay::setDecay(double decay){
    this->decay = decay;
}

void Delay::setMemory(int blocks){
    if(blocks == registerSize || blocks < 0){ //not resizing
        return;
    }
    circular_queue<double> newRegs(blocks, 0.);
    
    circular_queue<double>* smaller = NULL;
    circular_queue<double>* bigger = NULL;
    
    for(int i = 0; i < blocks && i < registers.size(); i++){
        newRegs[i] = registers[i];
    }
    
    registers = newRegs;
}


