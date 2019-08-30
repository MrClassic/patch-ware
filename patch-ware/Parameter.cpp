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
    this->param = NULL;
    patched = false;
}

Parameter::Parameter(double * const  param){
    this->param = param;
    patched = false;
}

void Parameter::setPtr(double * const ptr) {
	param = ptr;
}

double* Parameter::getPtr() const {
	return param;
}

bool Parameter::setParameter(const double param) {
	if (param == NULL)
		return false;
    if (patched) {
		if (!isReady())
			return false;
        *this->param = input();
		return true;
    } else {
        *this->param = param;
        return true;
    }
}

void Parameter::disconnect(){
    LinkedList<Patch> inputs = getInputPatches();
    while(!inputs.isEmpty()){
        Patch *pop = inputs.pop_front();
        if(pop != NULL){
            this->removeInput(pop);
            pop->getInput()->removeOutput(pop);
            pop->setOutput(NULL);
        }
        //delete pop; don't delete, let the Patch manager delete it!
    }
    patched = false;
}

bool Parameter::isPatched() const{
    return patched;
}

bool Parameter::process() {
	if (param == NULL)
		return false;
	if (isPatched()) {
		if (!isReady()) {
			return false;
		}
		*param = input();
	}
	return true;
}

//double operators
Parameter& Parameter::operator=(const double rhs){
    setParameter(rhs);
    return *this;
}
Parameter& Parameter::operator+=(const double rhs){
    setParameter(rhs + *param);
    return *this;
}
Parameter& Parameter::operator-=(const double rhs){
    setParameter(*param - rhs);
    return *this;
}

double Parameter::operator+(const double rhs) const{
	if (param == NULL)
		return rhs;
    return *param + rhs;
}
double Parameter::operator-(const double rhs) const{
	if (param == NULL)
		return rhs * -1.;
    return *param - rhs;
}
double Parameter::operator*(const double rhs) const{
	if (param == NULL)
		return 0.;
    return *param * rhs;
}
double Parameter::operator/(const double rhs) const{
	if (param == NULL)
		return 0.;
    return *param / rhs;
}

bool Parameter::operator==(const double rhs) const {
	if (param == NULL)
		return false;
    return *param == rhs;
}
bool Parameter::operator<(const double rhs) const {
	if (param == NULL)
		return false;
    return *param < rhs;
}
bool Parameter::operator>(const double rhs) const {
	if (param == NULL)
		return false;
    return *param > rhs;
}
bool Parameter::operator<=(const double rhs) const {
	if (param == NULL)
		return false;
    return *param <= rhs;
}
bool Parameter::operator>=(const double rhs) const {
	if (param == NULL)
		return false;
    return *param >= rhs;
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
Parameter& Parameter::operator=(Parameter &rhs){
	if (rhs.isPatched()) {
		LinkedList<Patch> patch = rhs.getInputPatches();
		Patch* pop = patch.pop_front();
		if (pop != NULL) {
			addInput(pop);
			rhs.removeInput(pop);
		}
	}
	else {
		setParameter(*rhs.param);
	}
    return *this;
}
Parameter& Parameter::operator+=(const Parameter &rhs){
    setParameter(*param + *rhs.param);
    return *this;
}
Parameter& Parameter::operator-=(const Parameter &rhs){
    setParameter(*param - *rhs.param);
    return *this;
}

double Parameter::operator+(const Parameter &rhs) const{
	if (param == NULL)
		return rhs;
    return *param + *rhs.param;
}
double Parameter::operator-(const Parameter &rhs) const{
	if (param == NULL)
		return rhs * -1.;
    return *param - *rhs.param;
}
double Parameter::operator*(const Parameter &rhs) const{
	if (param == NULL)
		return 0.;
    return *param * *rhs.param;
}
double Parameter::operator/(const Parameter &rhs) const{
	if (param == NULL)
		return 0.;
    return *param / *rhs.param;
}

bool Parameter::operator==(const Parameter &rhs) const {
	if (param == NULL)
		return false;
    return *param == *rhs.param;
}
bool Parameter::operator<(const Parameter &rhs) const {
	if (param == NULL)
		return false;
    return *param < *rhs.param;
}
bool Parameter::operator>(const Parameter &rhs) const {
	if (param == NULL)
		return false;
    return *param > *rhs.param;
}
bool Parameter::operator<=(const Parameter &rhs) const {
	if (param == NULL)
		return false;
    return *param <= *rhs.param;
}
bool Parameter::operator>=(const Parameter &rhs) const {
	if (param == NULL)
		return false;
    return *param >= *rhs.param;
}

Parameter::operator bool() const {
	if (param == NULL)
		return false;
    return *param > 0.;
}

Parameter::operator int() const {
	if (param == NULL)
		return 0;
	return (int)*param;
}

Parameter::operator double() const{
	if (param == NULL)
		return 0.;
    return *param;
}

bool Parameter::addInput(Patch * const patch) {
    if (patched) {
        return false;
    } else {
        disconnect();
        
		if (!DynamicInputDevice::addInput(patch)) {
			return false;
		}
		patched = true;
		return true;

    }
}

bool Parameter::removeInput(Patch * const patch) {
    if (patched) {
        LinkedList<Patch> patches = getInputPatches();
        //clear all input Patches (hopefully just one)
        while (!patches.isEmpty()) {
            DynamicInputDevice::removeInput(patches.pop_front());
        }
        patched = false;
		return true;
    } else {
		return false;
    }
}

double operator+(const double lhs, const Parameter &rhs){
    return rhs + lhs;
}
double operator-(const double lhs, const Parameter &rhs) {
	return lhs - (double)rhs;
}
double operator*(const double lhs, const Parameter &rhs) {
	return rhs * lhs;
}
double operator/(const double lhs, const Parameter &rhs) {
	return lhs / (double)rhs;
}