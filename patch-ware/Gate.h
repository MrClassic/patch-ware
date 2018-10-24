/*
File: Gate.h
Author: Nate Gallegos

Log:
7/28/18
File Created
basic implementation implemented
8/15/18
implemented Parameterizable interface
10/8/18
changed to implement the Signal Processor interface
*/

#ifndef GATE_H
#define GATE_H

#include "SignalProcessor.h"
#include "pwmath.h"

class Gate : public SignalProcessor {

public:

	/*
	Default Constructor, sets the threshold to 1
	*/
	Gate() : SignalProcessor() { 
		params.push_back(0.); //BYPASS
		params.push_back(0.); //THRESH
	}

	/*
	initialization constructor, initializes the threshold parameter.
	*/
	Gate(double thresh) : SignalProcessor(){
		params[THRESH] = thresh;
	}

	
	/*
	Proces, sends the input signal to the outputs if it is greater than the threshold,
	sends 0 if the signal is too low.
	*/
	double processSignal(const double &signal) {

		if (params[BYPASS] >= 1.)
			return signal;

		//gate logic
		
		if (pw_abs(signal) > pw_abs(params[THRESH])){
			//send signal through gate
			return signal;
		}
		else {
			//block signal (send 0)
			return 0.;
		}

	}

	enum parameters {
		BYPASS = 0,
		THRESH
	};
private:

	
};

#endif