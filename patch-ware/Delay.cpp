/* ************************************************************************
 * File:   Delay.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/5/17
 *      File Created
 *      Implemented core functionality
 *********************************************************************** */

#include "Delay.h"

Delay::Delay() {

	//set decay parameter
    params["decay"] = 0.5;
    
	//set regen parameter
	params["regen"] = 1.;

	//set mix parameter
	params["mix"] = 1.;

	//set registers
    circular_queue<double> temp(1, 0.);
    registers = temp;

	//set bypass
    params["bypass"] = false;
}

Delay::Delay(const Delay& orig) {

	//copy parameters
	for (auto it = orig.params.begin(); it != orig.params.end(); ++it) {
		params[it->first] = (double)it->second;
	}

	//copy registers
    registers = orig.registers;
}

Delay::~Delay() {/* Do Nothing */}

bool Delay::process(){

	//check inputs and parameters for ready state
    if(!*this || 
		(params["decay"].isPatched() && !params["decay"].isReady() ) ||
		(params["regen"].isPatched() && !params["regen"].isReady() ) )
        return false;
	
	//update parameters
    params["decay"].process();
	params["regen"].process();

	//set memory. If no change, the function backs out
	setMemory(params["regen"]);

	//get input signal
    double signal = input();

	//set default output
    double outSignal = params["mix"] * signal;

	//mix in stored signal
    outSignal += params["decay"] * registers.pop();

	//push output signal into memory registers
    registers.push(outSignal);

	//output signal
    output(outSignal);

	//success
    return true;
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


