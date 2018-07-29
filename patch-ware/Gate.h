/*
File: Gate.h
Author: Nate Gallegos

Log:
7/28/18
File Created
basic implementation implemented
*/

#ifndef GATE_H
#define GATE_H

#include "Effect.h"
#include "Parameter.h"
#include "pwmath.h"

class Gate : public Effect {

public:

	/*
	Default Constructor, sets the threshold to 1
	*/
	Gate() {
		threshold.setParameter(1.0);
	}

	/*
	initialization constructor, initializes the threshold parameter.
	*/
	Gate(double thresh) {
		threshold.setParameter(thresh);
	}

	/*
	Accessor for the threshold parameter by reference
	*/
	Parameter& getThreshold() {
		return threshold;
	}

	/*
	Proces, sends the input signal to the outputs if it is greater than the threshold,
	sends 0 if the signal is too low.
	*/
	bool process() {
		if (bypass) {
			//bypass gate
			output(input());
			return true;
		}

		//validate parameter
		if (!threshold.isReady()) {
			return false;
		}

		//gate logic
		double in = input();
		threshold.setParameter((double)threshold);
		if (pw_abs(in) > pw_abs((double)threshold)){
			//send signal through gate
			output(in);
		}
		else {
			//block signal (send 0)
			output(0.);
		}
		return true;
	}

private:

	//gate threshold. If the absolute value of the signal is not greater than the
	//threshold than the signal is blocked and o is output.
	Parameter threshold;
};

#endif