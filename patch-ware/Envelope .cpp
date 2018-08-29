/*
	File:	Envelope.cpp

	See "Envelope.h" for documentation/description

	History Log:

	8/20/18 - N8
		file created and documented
		Implemented base features

*/
#include "Envelope.h"

//default constructor, sets up default parameters and variables
Envelope::Envelope() {

	//boolean for whether the envelope is activated or not
	on = false;

	reset = false;//ignore duplicate threshold triggers

	//define general envelpoe parameters
	params["duration"] = 0.5; //duration in seconds
	params["base"] = 0.; //base signal, output when envelope is not active
	params["threshold"] = 1.; //activation threshold

}

//copies parameters
Envelope::Envelope(const Envelope &rhs) {
	copyParameters(rhs);
}

//overloaded Timer::incremntTime:
//ignores the time signal if the envelope is not active,
//updates the time if the envelope is active.
void Envelope::incrementTime(const double time) {
	if (!on) {
		//if not activated, ignore time change
		return;
	}
	if (currentTime + time > (double)params["duration"]) {
		//if envelope function ending, reset time and
		//turn off the envelope
		currentTime = 0.;
		on = false;
	}
	else {
		//envelope is active and the time is within
		//the envelope function domain, business as usual.
		currentTime += time;
	}
}

//implemented process method, calls abstract method:
//
//		double envelope(const double signal)
//
//if the envelope is active. outputs the base signal otherwise.
bool Envelope::process() {

	//test for readiness
	if (!*this || !parametersReady()) {
		return false;
	}

	updateParameters();

	double signal = input();//input signal
	double out = params["base"];//output signal

	//test envelope activation
	if (!on || reset) {
		if (signal >= (double)params["threshold"]) {
			//threshold exceeded, activate envelope
			on = true;
			currentTime = 0.;
		}
	}

	//if active, process envelope output
	if (on) {
		out = envelope(signal);//<--- virtual function 
								//		define in sub-class
	}

	output(out);
	return true;
}

//EOF