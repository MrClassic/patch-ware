/* ******************************************************************
 * File:   BiquadFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface through Effect
 ***************************************************************** */

#include "BiquadFilter.h"

BiquadFilter::BiquadFilter(){

	//make fir half
    fir = new FIRFilter;
    fir->setOrder(2);

	//make iir half
    iir = new IIRFilter;
    iir->setOrder(2);

	//set bypass
    params["bypass"] = false;
    fir->setBypass(false);
    iir->setBypass(false);

	//patch the halves together
    patch = new Patch;
    fir->addOutput(patch);
    iir->addInput(patch);
	
}

BiquadFilter::BiquadFilter(const BiquadFilter& orig) {
    if(fir != NULL){
        delete fir;
    }
    if(iir != NULL){
        delete iir;
    }
	if (patch != NULL) {
		delete patch;
	}

	//shallow copy
    fir = new FIRFilter(*orig.fir);
    iir = new IIRFilter(*orig.iir);
	patch = orig.patch;

	//copy over parameters (should just be "bypass")
	for (auto it = orig.params.begin(); it != orig.params.end(); it++) {
		params[it->first] = (double)it->second;
	}
}

BiquadFilter::~BiquadFilter() {

	//clean up your mess!!!
    delete patch;
    delete fir;
    delete iir;
}

bool BiquadFilter::process(){
	//updated bypass status for internal filters
    if(!params["bypass"]){
        iir->setBypass(false);
        fir->setBypass(false);
    }
    else{
        iir->setBypass(true);
        fir->setBypass(true);
    }
	// [signal] >>>> input >>>> fir >>>> iir >>>> output >>>>
    bool result = fir->process();
    if(result)
        result = iir->process();
    return result;
}

bool BiquadFilter::addOutput(Patch* const patch){
	//iir manages outputs
    return iir->addOutput(patch);
}
bool BiquadFilter::addInput(Patch* const patch){
	//fir manages inputs
    return fir->addInput(patch);
}
bool BiquadFilter::removeOutput(Patch* const patch){
	//iir manages outputs
    return iir->removeOutput(patch);
}
bool BiquadFilter::removeInput(Patch* const patch){
	//fir manages inputs
    return fir->removeInput(patch);
}

bool BiquadFilter::setParameter(const std::string &tag, const double value) {
	
	//handle "bypass" parameter
	//bypass is stored on the biquad itself, not the fir and iir
	if (tag == "bypass") {
		params["bypass"] = value;
		return true;
	}

	//handle the iir coefficients (a0, a1, a2)
	else if (tag[0] == 'a' || tag[0] == 'A') {
		std::string substr = tag.substr(1);
		if (iir->hasParameter(substr)) {
			return iir->setParameter(substr, value);
		}
		else {
			return false;
		}
	}

	//handle the fir coefficients (b0, b1, b2)
	else if (tag[0] == 'b' || tag[0] == 'B') {
		std::string substr = tag.substr(1);
		if (fir->hasParameter(substr)) {
			return fir->setParameter(substr, value);
		}
		else {
			return false;
		}
	}

	//if not "bypass" or an "a" or "b" coefficient, return false.
	return false;
}

bool BiquadFilter::getParameter(const std::string &tag, Parameter* &param) {

	//handle "bypass" parameter
	//bypass is stored on the biquad itself, not the fir and iir
	if (tag == "bypass") {
		param = &params["bypass"];
		return true;
	}

	//handle the iir coefficients (a0, a1, a2)
	else if (tag[0] == 'a' || tag[0] == 'A') {
		std::string substr = tag.substr(1);
		if (iir->hasParameter(substr)) {
			return iir->getParameter(substr, param);
		}
		else {
			return false;
		}
	}

	//handle the fir coefficients (b0, b1, b2)
	else if (tag[0] == 'b' || tag[0] == 'B') {
		std::string substr = tag.substr(1);
		if (fir->hasParameter(substr)) {
			return fir->getParameter(substr, param);
		}
		else {
			return false;
		}
	}

	//if not "bypass" or an "a" or "b" coefficient, return false.
	return false;
}

bool BiquadFilter::addParameter(const std::string &tag) {
	//Biquad requires only valid parameters, dont allow any others to be added
	return false;
}

bool BiquadFilter::hasParameter(const std::string &tag) const {
	
	//bypass is stored on the biquad itself, not the fir and iir
	if (tag == "bypass") {
		return true;
	}

	//handle the iir coefficients (a0, a1, a2)
	else if (tag[0] == 'a' || tag[0] == 'A') {
		std::string substr = tag.substr(1);
		if (iir->hasParameter(substr)) {
			return true;
		}
		else {
			return false;
		}
	}

	//handle the fir coefficients (b0, b1, b2)
	else if (tag[0] == 'b' || tag[0] == 'B') {
		std::string substr = tag.substr(1);
		if (fir->hasParameter(substr)) {
			return true;
		}
		else {
			return false;
		}
	}

	//if not "bypass" or an "a" or "b" coefficient, return false.
	return false;
}

LinkedList<Parameter> BiquadFilter::getParameters() {
	//simply returned both internal filters' lists of parameters
	return fir->getParameters() + iir->getParameters();
}


//EOF