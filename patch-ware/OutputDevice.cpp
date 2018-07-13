/* ********************************************************************** 
 * File:    OutputDevice.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/28/18
 *          File Created
 * 
 ********************************************************************** */

#include "OutputDevice.h"

OutputDevice::OutputDevice() { }

OutputDevice::OutputDevice(Patch * const patch) {
    addOutput(patch);
}

OutputDevice::OutputDevice(const OutputDevice &base) {
    LinkedList<Patch> copy = base.outputs;
    while (!copy.isEmpty()) {
        addOutput(copy.pop_front());
    }
}

bool OutputDevice::addOutput(Patch * const patch) {
    if (patch != NULL) {
        outputs.push_back(patch);
        patch->setInput(this);
    }
    return patch != NULL;
}

bool OutputDevice::removeOutput(Patch * const patch) {

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
        } else if (pop != patch) {
            //if not the patch to remove, re-insert it
            outputs.push_back(pop);
        } else {
            //if pop == patch, don't add. return true
            result = true;
            pop->setInput(NULL);
        }

    }

    //return result
    return result;

}

bool OutputDevice::checkOutputs() const {
    return outputs.apply(checkOutputsPrivate, NULL);
}

//      protected

void OutputDevice::output(const double signal) const {
    double sig = signal;
    outputs.apply(outputToPatches, &sig);
}

LinkedList<Patch> OutputDevice::getOutputPatches() const {
    return outputs;
}

//      Private

bool OutputDevice::outputToPatches(Patch* patch, void* arg) {
    double* signal = (double*) arg;
	if (!*patch)
		patch->pushSignal(*signal);
	else
		return false;
	return true;
}

bool OutputDevice::checkOutputsPrivate(Patch* patch, void* arg) {
    return !*patch;
}