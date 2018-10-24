


#include "SignalProcessor.h"

SignalProcessor::SignalProcessor() : Processor() {

}


SignalProcessor::~SignalProcessor() {
	//nothing to clean up
}

void SignalProcessor::process() {

	//condense inputs into a single signal
	double signal = condenseInputs();

	//run processing algorithm (virtual)
	signal = processSignal(signal);

	//push processed signal to outputs
	for (size_t output = 0; output < outputs.size(); output++) {
		*(outputs[output]) = signal;
	}
}