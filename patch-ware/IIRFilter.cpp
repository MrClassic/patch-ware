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

IIRFilter::IIRFilter() : Filter() {
    //setOrder(0);
    //bypass = true;
}

IIRFilter::IIRFilter(const IIRFilter& orig) : Filter(orig){

	//copy registers
	registers = orig.registers;

}

IIRFilter::~IIRFilter() { }

double IIRFilter::pushDouble(double signal){
    
    double out = 0.0;
    
    out += params[COEFFICIENTS] * signal;
    //int startIndex = regStart;
    for(int reg = 0; reg < registers.size(); reg++){
        out -= params[COEFFICIENTS + reg + 1] * registers[reg];
    }
    registers.push(out);
    
    return out;
}
