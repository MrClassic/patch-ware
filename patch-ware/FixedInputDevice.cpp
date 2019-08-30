/* **********************************************************************
 * File:    FixedInputDevice.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
 *          Constructors created
 *          2/28/18
 *          Core functionality added
 *			8/29/19
 *			Moved all logic into this file (FixedInputDevice.cpp)
 *			Adjusted to fit a vector instead of linked list
 ********************************************************************** */

#include "FixedInputDevice.h"

 /* ****************************************************************
  *                  Public Section
  **************************************************************** */

FixedInputDevice::FixedInputDevice() {
}

FixedInputDevice::FixedInputDevice(const unsigned int &channels) {
	inputs.resize(channels);
}

FixedInputDevice::FixedInputDevice(const FixedInputDevice &base) {
	
	//shallow copy patches over to new FixedInputDevice
	inputs.resize(base.inputs.size());
	for (int patch = 0; patch < inputs.size(); patch++) {
		inputs[patch] = base.inputs[patch];
	}

}

//O(1)
//insert into next available input.
//return false if all channels are full or NULL pointer is argument
bool FixedInputDevice::addInput(Patch * const patch) {
	if (patch != NULL) {
		for (int input = 0; input < inputs.size(); input++) {
			if (inputs[input] == NULL) {
				inputs[input] = patch;
				patch->setOutput(this);
				return true;
			}
		}
	}
	return false;
}

//attempts to insert the new patch into the specified input channel.
//whatever patch was there is returned (for clean up)
//if the channel is out of range, the orignal patch is returned back to the user of this function.
//can be used to remove a specific channel
Patch* FixedInputDevice::addInput(Patch * const patch, const unsigned int &channel) {
	if (channel >= inputs.size())
		return patch;
	Patch* pop = inputs[channel];
	inputs[channel] = patch;
	return pop;
}

//attempts to remove the patch argument from the list of inputs
bool FixedInputDevice::removeInput(Patch * const patch) {

	for (int input = 0; input < inputs.size(); input++) {
		if (inputs[input] == patch) {
			inputs[input] = NULL;
			return true;
		}
	}
	return false;
	
}

int FixedInputDevice::getInputCount() const {
	return inputs.size();
}

//adjusts the ammount of inputs allowed.
//any rejected Patch pointers are returned as a LinkedList.
LinkedList<Patch> FixedInputDevice::resizeInputChannels(const unsigned int &channels) {
	LinkedList<Patch> list;
	if (channels > inputs.size()) {
		inputs.resize(channels);
	}
	else {
		for (int input = channels; input < inputs.size(); input++) {
			list.push_back(inputs[input]);
		}
		inputs.resize(channels);
	}
	return list;
}

//O(n)
bool FixedInputDevice::isReady() const {
	
	bool allNull = true;
	for (int input = 0; input < inputs.size(); input++) {
		if (inputs[input] != NULL) {
			allNull = false;
			if (!inputs[input]->isReady()) {
				return false;
			}
		}
	}
	//all inputss == NULL -> return false
	//at least one input && all inputs ready -> return true
	return !allNull;
}



/* ****************************************************************
 *                  Protected Section
 **************************************************************** */

//creates a list of heap-allocated doubles. 
//dont forget to clear your memory you asshole
LinkedList<double> FixedInputDevice::getInputs() const {
	LinkedList<double> list;

	double* insert = NULL;
	for (int input = 0; input < inputs.size(); input++) {
		double request = 0.0;
		if (inputs[input]->requestSignal(request)) {
			//if signal request successful, add to list
			insert = new double;
			*insert = request;
			list.push_back(insert);
			insert = NULL;
		}
		else {
			//signal request failed, add NULL pointer to list
			list.push_back(NULL);
		}
	}

	return list;
}

LinkedList<Patch> FixedInputDevice::getInputPatches() {
	LinkedList<Patch> list;
	for (int input = 0; input < inputs.size(); input++) {
		list.push_back(inputs[input]);
	}
	return list;
}

/* ****************************************************************
 *                  Private Section
 **************************************************************** */
