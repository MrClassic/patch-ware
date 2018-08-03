/* ************************************************************************
 * File:   FIRFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/3/17
 *      File Created
 *      Implemented constructors and destructor
 *      Implemented the pushDouble method
 ************************************************************************ */


#include "FIRFilter.h"


FIRFilter::FIRFilter() {
	
}

FIRFilter::FIRFilter(const FIRFilter& orig) {
    
    bypass = orig.bypass;
    registers = orig.registers;
    
    //copy coefficients
    coefficients = new Parameter[registers.size() + 1];
    for(int i = 0; i <= registers.size(); i++){
        coefficients[i] = orig.coefficients[i];
    }
}

FIRFilter::~FIRFilter() { }

double FIRFilter::pushDouble(double signal){
    double outSignal = 0.0;
    if(!bypass){
        outSignal += coefficients[0] * signal;
        for(int reg = 0; reg < registers.size(); reg++){
            outSignal += coefficients[reg+1] * registers[reg];
        }
        registers.push(signal);
    }
    else{
        outSignal = signal;
    }
    
    return outSignal;
}

