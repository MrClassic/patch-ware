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
    
	//copy registers
    registers = orig.registers;
    
    //copy Parameters
	for (auto it = orig.params.begin(); it != orig.params.end(); ++it) {
		params[it->first] = (double)it->second;
	}
}

FIRFilter::~FIRFilter() { }

double FIRFilter::pushDouble(double signal){
    double outSignal = 0.0;
    if(!params["bypass"]){
        outSignal += params[std::to_string(0)] * signal;
        for(int reg = 0; reg < registers.size(); reg++){
            outSignal += params[std::to_string(reg+1)] * registers[reg];
        }
        registers.push(signal);
    }
    else{
        outSignal = signal;
    }
    
    return outSignal;
}

