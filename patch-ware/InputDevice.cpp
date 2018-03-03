/* ********************************************************************** 
 * File:    InputDevice.cpp
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
 *          Constructors created
 *          2/28/18
 *          Core functionality added
 ********************************************************************** */

#include "InputDevice.h"

/* ****************************************************************
 *                  Public Section
 **************************************************************** */

InputDevice::InputDevice() {
}

InputDevice::InputDevice(Patch * const patch) {
    addInput(patch);
}

InputDevice::InputDevice(const InputDevice &base) {
    LinkedList<Patch> copy = base.inputs;
    while (!copy.isEmpty()) {
        addInput(copy.pop_front());
    }
}

//O(1)

bool InputDevice::addInput(Patch * const patch) {
    if (patch != NULL) {
        inputs.push_back(patch);
        patch->setOutput(this);
    }
    return patch != NULL;
}

//O(2n)

bool InputDevice::removeInput(Patch * const patch) {

    //shallow copy inputs
    LinkedList<Patch> copy = inputs;

    //result to return, Patch removed: return true
    bool result = false;

    //shallow clear inputs
    inputs.clear();

    //copy Patches back into inputs
    Patch *pop = copy.pop_front();
    while (pop != NULL) {

        if (patch != pop) {
            //not the patch to remove, re-add it back into inputs
            inputs.push_back(pop);
        } else {
            //do not add the patch to "remove," return true
            result = true;
        }

        //pop next
        pop = copy.pop_front();
    }

    return result;
}

int InputDevice::getInputCount() const {
    return inputs.getSize();
}

bool InputDevice::checkInputs() const {
    return isReady();
}

//O(n)

bool InputDevice::isReady() const {
    if (inputs.isEmpty()) {
        return false;
    }
    
    //return result of applying checkReady on all input patches
    return inputs.apply(checkInputsPrivate, NULL);
}

//O(n)

InputDevice::operator bool() const {
    return isReady();
}

/* ****************************************************************
 *                  Protected Section
 **************************************************************** */

LinkedList<double> InputDevice::getInputs() const {
    LinkedList<double> list;
    inputs.apply(getInputsPrivate, &list);
    return list;
}

LinkedList<Patch> InputDevice::getInputPatches() const {
    return inputs;
}

double InputDevice::averageInputs() const {
    LinkedList<double> signals = getInputs();
    int size = signals.getSize();
    double sum = 0.;
    while (!signals.isEmpty()) {
        double* pop = signals.pop_front();
        if (pop == NULL) {
            size--;
        } else {
            sum += *pop;
            delete pop;
        }
    }
    if(size == 0.){
        return 0.;
    }
    return sum / size;
}

/* ****************************************************************
 *                  Private Section
 **************************************************************** */

//O(n)

bool InputDevice::checkInputsPrivate(Patch* patch, void *arg) {

    //if true, keep checking.
    //if false, stop looking and return false
    return (*patch);
}

bool InputDevice::getInputsPrivate(Patch* patch, void* arg) {
    LinkedList<double>* list = (LinkedList<double>*)arg;
    double* in = new double;
    if (patch->requestSignal(*in)) {
        list->push_back(in);
    } else {
        delete in;
    }
    return true;
}