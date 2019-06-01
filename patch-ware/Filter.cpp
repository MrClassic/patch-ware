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
 *		10/8/18
 *		Changed to Implement SignalProcessing interface
 ********************************************************************** */

#include "Filter.h"

/* ***********************************************************************
 * 
 *                      Filter implementation
 * 
 *********************************************************************** */
Filter::Filter() : SignalProcessor() {
	circular_stack<double> reg(1);
	registers = reg;
	registers[0] = 0.;
	params.push_back(0.); //BYPASS
	params.push_back(1.); //Coefficient 0
	params.push_back(0.); //Coefficient 1
}

Filter::~Filter(){
}

/* ***********************************************************************
 *                          Accessors
 ************************************************************************ */

int Filter::getOrder() const{
    return registers.size();
}

double* Filter::getCoefficient(int reg){
	if (reg >= params.size())
		return NULL;
	return &params[COEFFICIENTS + reg];
}


/* ***********************************************************************
 *                          Mutators
 ************************************************************************ */

double Filter::processSignal(const double &signal){
    
    
    //push signal regardless of bypass to update the filter
    double output = pushDouble(signal);

	//return filtered signal or original based on bypass state
    return (params[BYPASS] >= 1.) ? signal : output;
}

void Filter::setOrder(int order){
    
	if (order == registers.size() || order <= 0)
		return;

    //array to copy into
    circular_stack<double>newRegs(order);
	for (int i = 0; i < order; i++) {
		newRegs[i] = 0.;
	}
   
	//copy register values
	newRegs << registers;

	params.resize((size_t)order + 2);

	//set current registers to newRegs
	registers = newRegs;
}

bool Filter::setCoefficient(int reg, double coefficient){
    return setCoefficient(coefficient, reg);
}

bool Filter::setCoefficient(double coefficient, int reg){
    if(reg < 0 || reg > registers.size()){
        return false;
    }

    params[COEFFICIENTS + reg] = coefficient;

    return true;
}

void Filter::printCoefficients(std::ostream &os){
    for(int coe = 0; coe <= registers.size(); coe++){
        os << (double)(params[COEFFICIENTS + coe]) << ' ';
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


