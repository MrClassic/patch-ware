


#include "Processor.h"

Processor::Processor() {/* do nothing */}

Processor::Processor(const Processor &other) {
	inputs = other.inputs;
	outputs = other.outputs;
	params = other.params;
}

Processor::~Processor() { }

double* Processor::inputAddr(const int &input) {
	if (input >= inputs.size())
		return NULL;
	return &inputs[input];
}

double** Processor::outputAddr(const int &output) {
	if (output >= outputs.size())
		return NULL;
	return &outputs[output];
}

double* Processor::addInput() {
	inputs.push_back(0.);
	return &inputs[inputs.size() - 1];
}

bool Processor::removeOutput(double* const &remove) {
	for (auto it = outputs.begin(); it != outputs.end(); ++it) {
		if (remove == (*it)) {
			outputs.erase(it);
			return true;
		}
	}
	return false;
}

void Processor::addToOutputs(double* const &output) {
	outputs.push_back(output);
}

double* Processor::paramAddr(const int &param) {
	if (param >= params.size())
		return NULL;
	return &params[param];
}

bool Processor::setOutputChannel(const int &channel, double* const &output) {
	if (channel >= outputs.size()) {
		return false;
	}

	outputs[channel] = output;
	return true;
}

void Processor::setInputType(const input_type &_type) {
	type = _type;
}

void Processor::clear() {
	inputs.clear();
	outputs.clear();
	currentTime = 0.;
}

std::vector<double>& Processor::getInputs() {
	return inputs;
}

std::vector<double*>& Processor::getOutputs() {
	return outputs;
}

double Processor::condenseInputs() {

	if (inputs.empty())
		return 0.;

	double signal = inputs[0];

	switch (type) {
	case SUM:
		//add all inputs together
		for (size_t input = 1; input < inputs.size(); input++) {
			signal += inputs[input];
		}
		break;
	case PRODUCT:
		//multiply all inputs together
		for (size_t input = 1; input < inputs.size(); input++) {
			signal *= inputs[input];
		}
		break;
	case MIN:
		//get the minimum input
		for (size_t input = 1; input < inputs.size(); input++) {
			if (inputs[input] < signal)
				signal = inputs[input];
		}
		break;
	case MAX:
		//get the maximum input
		for (size_t input = 1; input < inputs.size(); input++) {
			if (inputs[input] > signal)
				signal = inputs[input];
		}
		break;
	case AVERAGE:
		//get the average of all inputs
		for (size_t input = 1; input < inputs.size(); input++) {
			signal += inputs[input];
		}
		signal /= inputs.size();
		break;
	}

	//return result
	return signal;

}