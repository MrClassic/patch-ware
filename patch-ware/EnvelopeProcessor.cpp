


#include "EnvelopeProcessor.h"

EnvelopeProcessor::EnvelopeProcessor() : SignalProcessor() {
	on = false;
	reset = false;
	params.reserve(NUM_PARAMS);
	params.push_back(1.); //THRESH
	params.push_back(0.); //BASE
	params.push_back(1.); //DURATION

}

EnvelopeProcessor::EnvelopeProcessor(const EnvelopeProcessor &orig) : SignalProcessor(orig){
	on = orig.on;
	reset = orig.reset;
}

EnvelopeProcessor::~EnvelopeProcessor() {
	//nothing to clean up
}

double EnvelopeProcessor::processSignal(const double &signal) {

	//check for signal to exceed (or meet) threshold trigger
	if (signal >= params[THRESH]) {
		on = true;
		if (reset) {
			currentTime = 0.;
		}
	}
	
	if(on) {

		//increment time
		currentTime += frameRate;

		//check to see if we are done
		if (currentTime > params[DURATION]) {
			on = false;
			currentTime = 0.;
			return params[BASE];
		}

		//run envelope function
		return envelope();
	}
	else {

		//bypass
		return params[BASE];
	}
}