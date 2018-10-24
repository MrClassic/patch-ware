/*
	File:	Envelope.cpp

	See "Envelope.h" for documentation/description

	History Log:

	8/20/18 - N8
		file created and documented
		Implemented base features

*/
#include "Envelope.h"

Envelope::Envelope() {
	proc = NULL;
}

//default constructor, sets up default parameters and variables
Envelope::Envelope(EnvelopeProcessor* eProc) {

	proc = eProc;

	//define general envelpoe parameters
	addParameter("duration", proc->paramAddr(EnvelopeProcessor::DURATION));
	params["duration"] = 0.5; //duration in seconds
	addParameter("base", proc->paramAddr(EnvelopeProcessor::BASE));
	params["base"] = 0.; //base signal, output when envelope is not active
	addParameter("threshold", proc->paramAddr(EnvelopeProcessor::THRESH));
	params["threshold"] = 1.; //activation threshold

}

//copies parameters
Envelope::Envelope(const Envelope &rhs) {
	copyParameters(rhs);
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

	output(proc->processSignal(signal));//process and output

	return true; 
}

void Envelope::incrementTime(const double time) {
	proc->frameRate = time;
}


//EOF