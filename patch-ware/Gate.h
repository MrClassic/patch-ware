/*
File: Gate.h
Author: Nate Gallegos

Log:
7/28/18
File Created
basic implementation implemented
8/15/18
implemented Parameterizable interface
*/

#ifndef GATE_H
#define GATE_H

#include "Effect.h"
#include "pwmath.h"

class Gate : public Effect {

public:

	/*
	Default Constructor, sets the threshold to 1
	*/
	Gate() {
		params["threshold"] = 1.;
	}

	/*
	initialization constructor, initializes the threshold parameter.
	*/
	Gate(double thresh) {
		params["threshold"] = thresh;
	}

	
	/*
	Proces, sends the input signal to the outputs if it is greater than the threshold,
	sends 0 if the signal is too low.
	*/
	bool process() {

		//test for bypass
		params["bypass"].process();
		if (params["bypass"]) {
			//bypass gate
			output(input());
			return true;
		}

		//validate parameter
		if (params["threshold"].isPatched() && !params["threshold"].isReady() || !*this) {
			return false;
		}

		//gate logic
		double in = input();
		params["threshold"].process();
		if (pw_abs(in) > pw_abs((double)params["threshold"])){
			//send signal through gate
			output(in);
		}
		else {
			//block signal (send 0)
			output(0.);
		}

		//success!
		return true;
	}

private:

	
};

#endif