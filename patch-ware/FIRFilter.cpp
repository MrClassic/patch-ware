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


FIRFilter::FIRFilter() : Filter() {
	
}

FIRFilter::FIRFilter(const FIRFilter& orig) : Filter(orig) {
    
	//copy registers
    registers = orig.registers;
    
}

FIRFilter::~FIRFilter() { }

double FIRFilter::pushDouble(double signal){

    double outSignal = 0.0;
    outSignal += params[COEFFICIENTS] * signal;
    for(int reg = 0; reg < registers.size(); reg++){
        outSignal += params[COEFFICIENTS + reg + 1] * registers[reg];
    }
    registers.push(signal);
    
    return outSignal;
}

