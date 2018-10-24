/*
File: PulseGenerator.h
Author: Nate Gallegos

Log:
	7/28/18
		File Created
		basic implementation implemented
	8/15/18
		implemented Parameterizable interface
	10/9/18
		changed to implement the WaveGenerator interface
*/

#ifndef PULSEGEN_H
#define PULSEGEN_H

#include "WaveGenerator.h"

class PulseGenerator : public WaveProcessor {

public:

	/*
	Default Constructor, sets pulse to true for the first frame of the wave generator
	*/
	PulseGenerator() {
		pulse = true;
	}

	/*
	generate, sends the amplitude of the wave if the pulse is ready,
	or 0 if the pulse is not ready.
	*/
	double generate() {
		
		//update time through fluxuating frequency
		if(params[FREQUENCY] != lastFreq)
			currentTime = (params[FREQUENCY] * currentTime) / lastFreq;

		if (pulse) {
			//pulse ready, output signal for one frame
			pulse = false;
			return params[AMPLITUDE];
		}
		else {
			//output 0 if pulse is not ready
			return 0.;
		}
	}

private:

	//boolean for outputting the pulse
	bool pulse;
};


#endif