/* ********************************************************************** 
 * File:    Patch.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          1/7/18
 *          File Created
 * 
 ********************************************************************** */

#include "Patch.h"

Patch::Patch(){
    output = NULL;
    input = NULL;
    ready = false;
}

Patch::Patch(const Patch& p){
    output = p.output;
    input = p.input;
    ready = false;
}

bool Patch::isReady() const{
    return ready;
}

InputDevice* Patch::getOutput() const{
    return output;
}

OutputDevice* Patch::getInput() const {
    return input;
}

bool Patch::pushSignal(const double signal){
    if(ready){
        return false;
    }
    this->signal = signal;
    ready = true;
    return true;
}

bool Patch::requestSignal(double& out){
    if(!ready){
        return false;
    }
    out = signal;
    ready = false;
    return true;
}

void Patch::setOutput(InputDevice* out){
    output = out;
}

void Patch::setInput(OutputDevice* in){
    input = in;
}

Patch::operator  bool() const{
    return isReady();
}