/* ***********************************************************************
 * File:   Filter.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/2/17
 *      File Created
 *      implemented basic accessors and mutators
 *      5/3/17
 *      finished implementing setOrder.
 ********************************************************************** */

#include "Patch.h"
#include "Circular.h"
#include "Filter.h"


/* ***********************************************************************
 * 
 *                      Filter implementation
 * 
 *********************************************************************** */

Filter::~Filter(){
    delete[] coefficients;
    coefficients = NULL;
}

/* ***********************************************************************
 *                          Accessors
 ************************************************************************ */

int Filter::getOrder() const{
    return registers.size();
}

Parameter& Filter::getCoefficient(int reg){
    if(reg >= registers.size() || reg < 0){
        return coefficients[0];
    }
    else{
        return coefficients[reg];
    }
}


/* ***********************************************************************
 *                          Mutators
 ************************************************************************ */

bool Filter::process(){
    //check my input channels for ready
    if(!*this){
        return false;
    }
    
    //check my parameters for ready
    for(int coe = 0; coe <= registers.size(); coe++){
        if(coefficients[coe].getInputCount() > 0 && !coefficients[coe].isReady()){
            return false;
        }
    }
    
    
    double signal = input();
    for(int coe = 0; coe <= registers.size(); coe++){
        coefficients[coe].setParameter(coefficients[coe]);
    }
    
    //output processed signal
    output(pushDouble(signal));
    return true;
}

void Filter::setOrder(int order){
    
    //array to copy into
    circular_stack<double>newRegs(order);
    Parameter *newCoes = new Parameter[order + 1];
    
    //making registers smaller
    if(order < this->registers.size()){
        
        newCoes[0] = coefficients[0];
        for(int reg = 0; reg < newRegs.size(); reg++){
            newRegs[reg] = registers[reg];
            newCoes[reg+1] = coefficients[reg+1];
        }
        for(int i = order; i < registers.size(); i++){
            coefficients[i].disconnect();
        }
        
    }
    //making registers larger
    else if(order > this->registers.size()){
        
        newCoes[0] = coefficients[0];
        for(int reg = 0; reg < registers.size(); reg++){
            newRegs[reg] = registers[reg];
            newCoes[reg+1] = coefficients[reg+1];
        }
        for(int i = registers.size(); i < order; i++){
            coefficients[i] = 1.0;
        }
    }
    //same number of registers, do nothing
    else{
        return;
    }
    
    //set registers to new array, deallocate old array, set regStart index
    registers = newRegs;
    
    //set new coefficients array
    Parameter* deleteMe = coefficients;
    coefficients = newCoes;
    newCoes = NULL;
    delete[] deleteMe;
    deleteMe = NULL;
}

bool Filter::setCoefficient(int reg, double coefficient){
    return setCoefficient(coefficient, reg);
}

bool Filter::setCoefficient(double coefficient, int reg){
    if(reg < 0 || reg > registers.size()){
        return false;
    }
    coefficients[reg] = coefficient;
    return true;
}

void Filter::printCoefficients(std::ostream &os) const{
    for(int coe = 0; coe <= registers.size(); coe++){
        os << (double&)coefficients[coe] << ' ';
    }
}

void Filter::printRegisters(std::ostream& os) const{
    if(registers.size() == 0){
        os << "No registers to display";
    }
    for(int reg = 0; reg < registers.size(); reg++){
        os << (double)registers[reg] << ' ';
    }
        
}


