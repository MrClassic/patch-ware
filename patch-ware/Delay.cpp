/* ************************************************************************
 * File:   Delay.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/5/17
 *      File Created
 *      Implemented core functionality
 *		10/8/18
 *		Changed to implement SignalProcessor interface
 *********************************************************************** */

#include "Delay.h"

Delay::Delay() {

	params.resize(NUM_PARAMS);

	//set decay parameter
    params[DECAY] = 0.5;
    
	//set regen parameter
	params[REGEN] = 1.;

	//set mix parameter
	params[MIX] = 1.;

	//set registers
    circular_queue<double> temp(1, 0.);
    registers = temp;

	//set bypass
    params[BYPASS] = false;
}

Delay::Delay(const Delay& orig) : SignalProcessor(orig) {

	//copy registers
    registers = orig.registers;
}

Delay::~Delay() {/* Do Nothing */}

double Delay::processSignal(const double &signal){

	//set memory. If no change, the function backs out
	setMemory((int)params[REGEN]);

	//set default output
    double outSignal = params[MIX] * signal;

	//mix in stored signal
	double pop = 0.;
	registers.pop(pop);
    outSignal += params[DECAY] * pop;

	//push output signal into memory registers
    registers.push(outSignal);

	//output signal (or original signal if bypassed)
    return (params[BYPASS] >= 1.) ? signal : outSignal;

}


void Delay::setMemory(int blocks){
    if(blocks == registers.size() || blocks <= 0){ //not resizing
        return;
    }

	//new register set
    circular_queue<double> newRegs(blocks, 0.);
    
	//copy over populated registers
	newRegs << registers;
    
	//set to new registers
    registers = newRegs;
}


