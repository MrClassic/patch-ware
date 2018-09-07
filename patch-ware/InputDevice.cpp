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

	return inputs.remove(patch);
	/*
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
			pop->setOutput(NULL);
            result = true;
        }

        //pop next
        pop = copy.pop_front();
    }

    return result;
	*/
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

void InputDevice::setInputType(input_type in){
    inputType = in;
}

/* ****************************************************************
 *                  Protected Section
 **************************************************************** */

LinkedList<double> InputDevice::getInputs() const {
    LinkedList<double> list;
    inputs.apply(getInputsPrivate, &list);
    return list;
}

LinkedList<Patch> InputDevice::getInputPatches() {
    return inputs;
}

double InputDevice::input() const{
    double out = 0.;
    switch(inputType){
        
        case SUM:
            out = sumInputs();
        break;
        
        case MAX:
            out = maxInput();
        break;
        
        case MIN:
            out = minInput();
        break;
        
        case PRODUCT:
            out = multiplyInputs();
        break;
            
        case AVERAGE:
        default:
            out = averageInputs();
    }
    return out;
}

double InputDevice::averageInputs() const {
    LinkedList<double> signals = getInputs(); //<--- creates doubles for list on the heap
    int size = signals.getSize();
    double sum = 0.;
    while (!signals.isEmpty()) {
        double* pop = signals.pop_front();
        if (pop == NULL) {
            size--;
        } else {
            sum += *pop;
            delete pop; //<---- free heap memory
        }
    }
    if(size == 0){
        return 0.;
    }
    return sum / size;
}

double InputDevice::maxInput() const {
    LinkedList<double> signals = getInputs();
    double* pop = signals.pop_front();
    if(pop == NULL){
        return 0.;
    }
    double largest = *pop;
    delete pop;
    pop = NULL;
    while (!signals.isEmpty()) {
        pop = signals.pop_front();
        if(pop != NULL) {
            if(*pop > largest){
                largest = *pop;
            }
            delete pop;
        }
    }
    return largest;
}

double InputDevice::minInput() const {
    LinkedList<double> signals = getInputs();
    double* pop = signals.pop_front();
    if(pop == NULL){
        return 0.;
    }
    double smallest = *pop;
    delete pop;
    pop = NULL;
    while (!signals.isEmpty()) {
        pop = signals.pop_front();
        if(pop != NULL) {
            if(*pop < smallest){
                smallest = *pop;
            }
            delete pop;
        }
    }
    return smallest;
}

double InputDevice::sumInputs() const {
    LinkedList<double> signals = getInputs();
    double sum = 0.;
    while (!signals.isEmpty()) {
        double* pop = signals.pop_front();
        if (pop == NULL) {
            // ...oops, how did that get there?
        } else {
            sum += *pop;
            delete pop;
        }
    }
    return sum;
}

double InputDevice::multiplyInputs() const {
    LinkedList<double> signals = getInputs();
    double total = 1.;
    while (!signals.isEmpty()) {
        double* pop = signals.pop_front();
        if (pop == NULL) {
            // ...
        } else {
            total *= *pop;
            delete pop;
        }
    }
    return total;
}

/* ****************************************************************
 *                  Private Section
 **************************************************************** */

//O(n)
//Function to pass to LinkedList.apply((bool*)(Patch*, void*), void*)
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