/*
File: PulseGenerator.h
Author: Nate Gallegos

Log:
	7/28/18
		File Created
		basic implementation implemented
*/

#ifndef PULSE_H
#define PULSE_H

#include "WaveGenerator.h"

class PulseGenerator : WaveGenerator {

public:

	/*
	Default Constructor, sets pulse to true for the first frame of the wave generator
	*/
	PulseGenerator() {
		pulse = true;
	}

	/*
	Overloaded virtual method incrementTime, checks for time overflow and sets the pulse boolean.
	time overflow means the wave has started over, thus its ready to send it's pulse signal again.
	*/
	void incrementTime(const double time) {
		//update pulse boolean if wave repeats (time overflows)
		if (currentTime + time > (1. / (double)frequency)) {
			pulse = true;
		}
		//increment time via standard wave generator
		WaveGenerator::incrementTime(time);
	}

	/*
	Push Double, sends the amplitude of the wave if the pulse is ready,
	or 0 if the pulse is not ready.
	*/
	bool pushDouble() {
		//check parameters
		if (!paramsReady()) {
			return false;
		}

		if (pulse) {
			//pulse ready, output signal for one frame
			pulse = false;
			amplitude.setParameter((double)amplitude);
			output((double)amplitude);
		}
		else {
			//output 0 if pulse is not ready
			output(0.);
		}
		return true;
	}

private:

	//boolean for outputting the pulse
	bool pulse;
};


#endif