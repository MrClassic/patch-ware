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

	//copy registers
	registers = orig.registers;

	//copy Parameters
	for (auto it = orig.params.begin(); it != orig.params.end(); ++it) {
		params[it->first] = (double)it->second;
	}
}

IIRFilter::~IIRFilter() { }

double IIRFilter::pushDouble(double signal){
    
    double out = 0.0;
    if(!params["bypass"]){
        out += params["0"] * signal;
        //int startIndex = regStart;
        for(int reg = 0; reg < registers.size(); reg++){
            out += params[std::to_string(reg+1)] * registers[reg];
        }
        registers.push(out);
    }
    else{
        out = signal;
    }
    
    return out;
}
