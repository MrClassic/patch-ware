/* **********************************************************************
 * File:    FixedOutputDevice.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          8/29/19
 *          File Created
 ********************************************************************** */

#include "FixedOutputDevice.h"

 /*
 Default Constructor: Does nothing special. nothing to initialize.
 */
FixedOutputDevice::FixedOutputDevice() { }

/*
Initialization Constructor: Accepts a Patch to initialize as an ouput.
*/
FixedOutputDevice::FixedOutputDevice(const unsigned int &channels) {
	//addOutput(patch);
	outputs.resize(channels);
}

/*
Copy Constructor: Performs a shallow copy of the base OutputDevice's output Patches.
*/
FixedOutputDevice::FixedOutputDevice(const FixedOutputDevice &base) {
	outputs.resize(base.outputs.size());
	for (int output = 0; output < outputs.size(); output++) {
		outputs[output] = base.outputs[output];
	}
}

/*
Add Output: Attempts to add a patch to this output device's output channels.
*/
bool FixedOutputDevice::addOutput(Patch * const patch) {
	if (patch != NULL) {
		for (int channel = 0; channel < outputs.size(); channel++) {
			if (outputs[channel] == NULL) {
				outputs[channel] = patch;
				patch->setInput(this);
				return true;
			}
		}
	}
	return false;
}

Patch* FixedOutputDevice::addOutput(Patch * const patch, const unsigned int &channel) {
	if (channel >= outputs.size()) {
		return NULL;
	}

	Patch* pop = outputs[channel];
	outputs[channel] = patch;
	return pop;
}

/*
Remove Output: Attempts to remove the specified Patch from this output device's
output Patches. If the patch is removed, true is returned. If the patch was
not in this output device's output channels, false is returned.
*/
bool FixedOutputDevice::removeOutput(Patch * const patch) {

	for (int channel = 0; channel < outputs.size(); channel++) {
		if (outputs[channel] == patch) {
			outputs[channel] = NULL;
			return true;
		}
	}
	return false;

}

LinkedList<Patch> FixedOutputDevice::resizeOutputChannels(const unsigned int &channels) {
	LinkedList<Patch> list;
	if (channels > outputs.size()) {
		outputs.resize(channels);
	}
	else {
		for (int input = channels; input < outputs.size(); input++) {
			list.push_back(outputs[input]);
		}
		outputs.resize(channels);
	}
	return list;
}

/*
Gets the count of output channels for this OutputDevice
*/
int FixedOutputDevice::getOutputCount() const {
	return outputs.size();
}

/*
Check Outputs: Checks whether the output patches are ready to accept the next signal.
If there are any output patches that are not ready or all output patches are NULL, false is returned.
If all output patches are ready, then true is returned.
*/
bool FixedOutputDevice::checkOutputs() const {
	bool allNull = true;
	for (int channel = 0; channel < outputs.size(); channel++) {
		if (outputs[channel] != NULL) {
			allNull = false;
			if (!outputs[channel]->isReady()) {
				return false;
			}
		}
	}
	return !allNull;
}

//      protected

/*
Output: Sends the specified signal to the output patches.
*/
void FixedOutputDevice::output(const double signal) const {
	for (int channel = 0; channel < outputs.size(); channel++) {
		if (outputs[channel] != NULL) {
			if (!outputs[channel]->pushSignal(signal)) {
				// !! failed pushing signal. Hmmmm... !!
			}
		}
	}
}

/*
Output: Sends the signals to thier respective output channels.
i.e.	signals[i] -> outputs[i]
Any output channel that is not included in the signals vector will have 0.0's pushed into them.
*/
void FixedOutputDevice::output(const std::vector<double> signals) const{
	for (int channel = 0; channel < outputs.size(); channel++) {
		if (outputs[channel] != NULL) {
			if (channel >= signals.size()) {
				if (!outputs[channel]->pushSignal(0.0)) {
					// !! failed pushing signal. Hmmmm... !!
				}
			}
			else {
				if (!outputs[channel]->pushSignal(signals[channel])) {
					// !! failed pushing signal. Hmmmm... !!
				}
			}
		}
	}

}

/*
Get Output Patches: Accessor for this Output Device's output Patches
in the form of a LinkedList. Returns a shallow copy of the output
patche pointers
*/
LinkedList<Patch> FixedOutputDevice::getOutputPatches() {
	LinkedList<Patch> list;
	for (Patch* p : outputs) {
		list.push_back(p);
	}
	return list;
}

