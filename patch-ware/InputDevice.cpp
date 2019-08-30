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
#include <float.h>

/* ****************************************************************
 *                  Public Section
 **************************************************************** */

InputDevice::InputDevice() {
}

bool InputDevice::checkInputs() const {
    return isReady();
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
    
    double largest = -DBL_MAX;
    double* pop = NULL;
    while (!signals.isEmpty()) {
        pop = signals.pop_front();
        if(pop != NULL) {
            if(*pop > largest){
                largest = *pop;
            }
            delete pop;
        }
    }
    return largest != -DBL_MAX ? largest : 0.;
}

double InputDevice::minInput() const {
    LinkedList<double> signals = getInputs();
    double* pop = NULL;
    double smallest = DBL_MAX;
    while (!signals.isEmpty()) {
        pop = signals.pop_front();
        if(pop != NULL) {
            if(*pop < smallest){
                smallest = *pop;
            }
            delete pop;
        }
    }
	return  smallest != DBL_MAX ? smallest : 0.;
}

double InputDevice::sumInputs() const {
    LinkedList<double> signals = getInputs();
    double sum = 0.;
    while (!signals.isEmpty()) {
        double* pop = signals.pop_front();
        if (pop == NULL) {
            // nothing to see here...
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
            // nothing to see here...
        } else {
            total *= *pop;
            delete pop;
        }
    }
    return total;
}

