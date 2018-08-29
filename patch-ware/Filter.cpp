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

#include "Filter.h"

/* ***********************************************************************
 * 
 *                      Filter implementation
 * 
 *********************************************************************** */
Filter::Filter() {
	circular_stack<double> reg(1);
	registers = reg;
	registers[0] = 0.;
	params[std::to_string(0)] = 1.;
	params[std::to_string(1)] = 0.;
}

Filter::~Filter(){
    //delete[] coefficients;
    //coefficients = NULL;
}

/* ***********************************************************************
 *                          Accessors
 ************************************************************************ */

int Filter::getOrder() const{
    return registers.size();
}

Parameter& Filter::getCoefficient(int reg){
	std::string asString = std::to_string(reg);
	if (!hasParameter(asString)) {
		return params["0"];
	}
	return params[asString];
}


/* ***********************************************************************
 *                          Mutators
 ************************************************************************ */

bool Filter::process(){
    //check my input channels for ready
    if(!isReady()){
        return false;
    }
    
    //check my parameters for ready
    for(int coe = 0; coe <= registers.size(); coe++){
		std::string asString = std::to_string(coe);
        if(params[asString].isPatched() && !params[asString].isReady()){
            return false;
        }
    }
    
    
    double signal = input();
    for(int coe = 0; coe <= registers.size(); coe++){
        params[std::to_string(coe)].process();
    }
    
    //output processed signal
    output(pushDouble(signal));
    return true;
}

void Filter::setOrder(int order){
    
    //array to copy into
    circular_stack<double>newRegs(order);
   
    //making registers smaller
    if(order < this->registers.size()){
        
        for(int reg = 0; reg < newRegs.size(); reg++){
            newRegs[reg] = registers[reg];
        }
        for(int i = order; i < registers.size(); i++){
			params.erase(std::to_string(i + 1));
        }
        
    }

    //making registers larger
    else if(order > this->registers.size()){
        
        for(int reg = 0; reg < registers.size(); reg++){
            newRegs[reg] = registers[reg];
        }
        for(int i = registers.size(); i < order; i++){
			newRegs[i] = 0.;
            params[std::to_string(i + 1)].setParameter(1.0);
        }
    }

    //same number of registers, do nothing
    else{
        return;
    }
    
    //set registers to new array, deallocate old array, set regStart index
    registers = newRegs;
    
}

bool Filter::setCoefficient(int reg, double coefficient){
    return setCoefficient(coefficient, reg);
}

bool Filter::setCoefficient(double coefficient, int reg){
    if(reg < 0 || reg > registers.size()){
        return false;
    }
    params[std::to_string(reg)].setParameter(coefficient);
    return true;
}

void Filter::printCoefficients(std::ostream &os){
    for(int coe = 0; coe <= registers.size(); coe++){
        os << (double)(params[std::to_string(coe)]) << ' ';
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


