/* **********************************************************************
 * File:    DynamicInputDevice.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
 *          Constructors created
 *          2/28/18
 *          Core functionality added
 *			8/29/19
 *			Moved all logic into this file (DynamicInputDevice.cpp)
 *				from InputDevice.cpp
 ********************************************************************** */

#include "DynamicInputDevice.h"

 /* ****************************************************************
  *                  Public Section
  **************************************************************** */

DynamicInputDevice::DynamicInputDevice() {
}

DynamicInputDevice::DynamicInputDevice(Patch * const patch) {
	addInput(patch);
}

DynamicInputDevice::DynamicInputDevice(const DynamicInputDevice &base) {
	LinkedList<Patch> copy = base.inputs;
	while (!copy.isEmpty()) {
		addInput(copy.pop_front());
	}
}

//O(1)

bool DynamicInputDevice::addInput(Patch * const patch) {
	if (patch != NULL) {
		inputs.push_back(patch);
		patch->setOutput(this);
	}
	return patch != NULL;
}

//O(2n)

bool DynamicInputDevice::removeInput(Patch * const patch) {

	return inputs.remove(patch);
	
}

int DynamicInputDevice::getInputCount() const {
	return inputs.getSize();
}


//O(n)

bool DynamicInputDevice::isReady() const {
	if (inputs.isEmpty()) {
		return false;
	}

	//return result of applying checkReady on all input patches
	return inputs.apply(checkInputsPrivate, NULL);
}



/* ****************************************************************
 *                  Protected Section
 **************************************************************** */

LinkedList<double> DynamicInputDevice::getInputs() const {
	LinkedList<double> list;
	inputs.apply(getInputsPrivate, &list);
	return list;
}

LinkedList<Patch> DynamicInputDevice::getInputPatches() {
	return inputs;
}

/* ****************************************************************
 *                  Private Section
 **************************************************************** */

 //O(n)
 //Function to pass to LinkedList.apply((bool*)(Patch*, void*), void*)
bool DynamicInputDevice::checkInputsPrivate(Patch* patch, void *arg) {

	//if true, keep checking.
	//if false, stop looking and return false
	return (*patch);
}

bool DynamicInputDevice::getInputsPrivate(Patch* patch, void* arg) {
	LinkedList<double>* list = (LinkedList<double>*)arg;
	double* in = new double;
	if (patch->requestSignal(*in)) {
		list->push_back(in);
	}
	else {
		delete in;
	}
	return true;
}