/* ************************************************************************
 * File:   IIRFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/4/17
 *      File Created
 *      Implemented constructors and destructor
 *      Implemented the pushDouble method
 ************************************************************************ */

#include "IIRFilter.h"

IIRFilter::IIRFilter() {
    //setOrder(0);
    //bypass = true;
}

IIRFilter::IIRFilter(const IIRFilter& orig) {
    bypass = orig.bypass;
    registers = orig.registers;
    
    //copy coefficients
    coefficients = new Parameter[registers.size() + 1];
    for(int i = 0; i <= registers.size(); i++){
        coefficients[i] = orig.coefficients[i];
    }
}

IIRFilter::~IIRFilter() { }

double IIRFilter::pushDouble(double signal){
    
    double out = 0.0;
    if(!bypass){
        out += coefficients[0] * signal;
        //int startIndex = regStart;
        for(int reg = 0; reg < registers.size(); reg++){
            out += coefficients[reg+1] * registers[reg];
        }
        registers.push(out);
    }
    else{
        out = signal;
    }
    
    return out;
}
