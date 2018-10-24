

#include "WaveProcessor.h"

WaveProcessor::WaveProcessor() {
	params.reserve(NUM_PARAMS);
	params.push_back(0.5); //Frequency
	params.push_back(1.0); //Amplitude
	params.push_back(0.);  //Phase

	lastFreq = params[FREQUENCY];
	lastAmp = params[AMPLITUDE];
	lastPhase = params[PHASE];
}

WaveProcessor::WaveProcessor(const WaveProcessor &orig) {
	lastAmp = orig.lastAmp;
	lastFreq = orig.lastFreq;
	lastPhase = orig.lastPhase;
	currentTime = orig.currentTime;
}


WaveProcessor::~WaveProcessor() {
	/*Nothing to clean up*/
}

double WaveProcessor::getPhaseOffset() {
	if ((double)params[FREQUENCY] == 0.) {
		return 0.;
	}

	updateWaveOffset();
	double out = (params[PHASE] + phaseCorrector);
	while (out > (1.)) {
		out -= (1.);
	}
	while (out < 0.) {
		out += (1.);
	}
	return out;
}

void WaveProcessor::process() {

	//increment time
	currentTime += 1./frameRate;

	//wrap time around wave time
	if (currentTime >= 1. / params[FREQUENCY]) {
		currentTime -= 1. / params[FREQUENCY];
	}

	double out = generate();

	//update "last___" variables
	//may be used in phase/amplitude correction equations.
	lastFreq = params[FREQUENCY];
	lastAmp = params[AMPLITUDE];
	lastPhase = params[PHASE];

	//send outputs
	for (int output = 0; output < outputs.size(); ++output) {
		*outputs[output] = out;
	}

}

//EOF