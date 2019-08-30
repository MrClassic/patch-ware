/* **********************************************************************
 * File:    DynamicOutputDevice.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          8/29/19
 *          File Created
 ********************************************************************** */

#include "DynamicOutputDevice.h"

 /*
 Default Constructor: Does nothing special. nothing to initialize.
 */
DynamicOutputDevice::DynamicOutputDevice() { }

/*
Initialization Constructor: Accepts a Patch to initialize as an ouput.
*/
DynamicOutputDevice::DynamicOutputDevice(Patch * const patch) {
	addOutput(patch);
}

/*
Copy Constructor: Performs a shallow copy of the base OutputDevice's output Patches.
*/
DynamicOutputDevice::DynamicOutputDevice(const DynamicOutputDevice &base) {
	LinkedList<Patch> copy = base.outputs;
	while (!copy.isEmpty()) {
		addOutput(copy.pop_front());
	}
}

/*
Add Output: Attempts to add a patch to this output device's output channels.
*/
bool DynamicOutputDevice::addOutput(Patch * const patch) {
	if (patch != NULL) {
		outputs.push_back(patch);
		patch->setInput(this);
	}
	return patch != NULL;
}

/*
Remove Output: Attempts to remove the specified Patch from this output device's
output Patches. If the patch is removed, true is returned. If the patch was
not in this output device's output channels, false is returned.
*/
bool DynamicOutputDevice::removeOutput(Patch * const patch) {

	//shallow copy outputs
	LinkedList<Patch> copy = outputs;

	//shallow clear
	outputs.clear();

	//result to return
	bool result = false;

	while (!copy.isEmpty()) {
		//Attempt to pop fro copied list
		Patch* pop = copy.pop_front();

		if (pop == NULL) {
			//if popped a NULL, don't re-insert... "how did that get there???"
			continue;
		}
		else if (pop != patch) {
			//if not the patch to remove, re-insert it
			outputs.push_back(pop);
		}
		else {
			//if pop == patch, don't add. return true
			result = true;
			pop->setInput(NULL);
		}

	}

	//return result
	return result;

}

/*
Gets the count of output channels for this OutputDevice
*/
int DynamicOutputDevice::getOutputCount() const {
	return outputs.getSize();
}

/*
Check Outputs: Checks whether the output patches are ready to accept the next signal.
If there are any output patches that are not ready, false is returned.
If all output patches are ready, then true is returned.
*/
bool DynamicOutputDevice::checkOutputs() const {
	return outputs.apply(checkOutputsPrivate, NULL);
}

//      protected

/*
Output: Sends the specified signal to the output patches.
*/
void DynamicOutputDevice::output(const double signal) const {
	double sig = signal;
	outputs.apply(outputToPatches, &sig);
}

/*
Get Output Patches: Accessor for this Output Device's output Patches
in the form of a LinkedList. Returns a shallow copy of the output
patches linked list
*/
LinkedList<Patch> DynamicOutputDevice::getOutputPatches() {
	return outputs;
}

//      Private

/*
Output To Patches: Private function to be sent to the LinkedList.apply() method.
This function converts the void* to a double, and sends it to the patch parameter.
The boolean returned is passed back to the LinkedList.apply() function to let it
know if it should continue applying this function to the next Patch in the list.
*/
bool DynamicOutputDevice::outputToPatches(Patch* patch, void* arg) {
	double* signal = (double*)arg;
	if (!*patch)
		return patch->pushSignal(*signal);
	return false;
}

/*
Check Outputs Private: Private function to be sent to the LinkedList.apply() method.
This function just checks the current Patch to see if it is ready for its next signal.
The boolean returned is passed back to the LinkedList.apply() function to let it
know if it should continue applying this function to the next Patch in the list.
*/
bool DynamicOutputDevice::checkOutputsPrivate(Patch* patch, void* arg) {
	return !*patch;
}