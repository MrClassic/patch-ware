/* ************************************************************************
 * File:   Parameter.cpp
 * Author: Nate Gallegos
 *
 * Log
 *      2/28/18
 *      File Created
 ************************************************************************ */

#include "Parameter.h"

Parameter::Parameter() {
    this->param = 0.;
    patched = false;
}

Parameter::Parameter(double param){
    this->param = param;
    patched = false;
}

bool Parameter::setParameter(const double param) {
    if (patched) {
        LinkedList<double> signals = getInputs();
        double* pop = signals.pop_front();
        if (pop == NULL) {
            this->param = param;
            signals.clear(true);
            return true;
        } else {
            this->param = *pop;
            signals.clear(true);
            return false;
        }
    } else {
        this->param = param;
        return true;
    }
}

void Parameter::disconnect(){
    LinkedList<Patch> inputs = getInputPatches();
    while(!inputs.isEmpty()){
        Patch *pop = inputs.pop_front();
        if(pop != NULL){
            pop->getInput()->removeOutput(pop);
            pop->setOutput(NULL);
        }
        //delete pop; don't delete, let the Patch manager delete it
    }
    patched = false;
}

//double operators
Parameter& Parameter::operator=(const double rhs){
    setParameter(rhs);
    return *this;
}
Parameter& Parameter::operator+=(const double rhs){
    setParameter(rhs + param);
    return *this;
}
Parameter& Parameter::operator-=(const double rhs){
    setParameter(param - rhs);
    return *this;
}

double Parameter::operator+(const double rhs) const{
    return param + rhs;
}
double Parameter::operator-(const double rhs) const{
    return param - rhs;
}
double Parameter::operator*(const double rhs) const{
    return param * rhs;
}
double Parameter::operator/(const double rhs) const{
    return param / rhs;
}

bool Parameter::operator==(const double rhs) const {
    return param == rhs;
}
bool Parameter::operator<(const double rhs) const {
    return param < rhs;
}
bool Parameter::operator>(const double rhs) const {
    return param > rhs;
}
bool Parameter::operator<=(const double rhs) const {
    return param <= rhs;
}
bool Parameter::operator>=(const double rhs) const {
    return param >= rhs;
}

Parameter& Parameter::operator=(const bool rhs){
    if(rhs){
        setParameter(1.);
    }
    else{
        setParameter(0.);
    }
    return *this;
}

//Parameter operators
Parameter& Parameter::operator=(const Parameter &rhs){
    setParameter(rhs.param);
    return *this;
}
Parameter& Parameter::operator+=(const Parameter &rhs){
    setParameter(param + rhs.param);
    return *this;
}
Parameter& Parameter::operator-=(const Parameter &rhs){
    setParameter(param - rhs.param);
    return *this;
}

double Parameter::operator+(const Parameter &rhs) const{
    return param + rhs.param;
}
double Parameter::operator-(const Parameter &rhs) const{
    return param - rhs.param;
}
double Parameter::operator*(const Parameter &rhs) const{
    return param * rhs.param;
}
double Parameter::operator/(const Parameter &rhs) const{
    return param / rhs.param;
}

bool Parameter::operator==(const Parameter &rhs) const {
    return param == rhs.param;
}
bool Parameter::operator<(const Parameter &rhs) const {
    return param < rhs.param;
}
bool Parameter::operator>(const Parameter &rhs) const {
    return param > rhs.param;
}
bool Parameter::operator<=(const Parameter &rhs) const {
    return param <= rhs.param;
}
bool Parameter::operator>=(const Parameter &rhs) const {
    return param >= rhs.param;
}

Parameter::operator bool() const {
    return param > 0;
}

Parameter::operator double() const {
    return param;
}

bool Parameter::addInput(Patch * const patch) {
    if (patched) {
        return false;
    } else {
        patched = true;
        return InputDevice::addInput(patch);
    }
}

bool Parameter::removeInput(Patch * const patch) {
    if (patched) {
        LinkedList<Patch> patches = getInputPatches();
        //clear all input Patches (hopefully just one)
        while (!patches.isEmpty()) {
            InputDevice::removeInput(patches.pop_front());
        }
        patched = false;
    } else {
        return false;
    }
}

double operator+(const double lhs, const Parameter &rhs){
    return rhs + lhs;
}
    double operator-(const double lhs, const Parameter &rhs){
        return lhs - (double)rhs;
    }
    double operator*(const double lhs, const Parameter &rhs){
        return rhs * lhs;
    }
    double operator/(const double lhs, const Parameter &rhs){
        return lhs / (double)rhs;
    }