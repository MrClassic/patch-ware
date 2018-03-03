/* ******************************************************************
 * File:   BiquadFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/11/17
 *      File Created
 ***************************************************************** */

#include "BiquadFilter.h"

BiquadFilter::BiquadFilter(){
    fir = new FIRFilter;
    fir->setOrder(2);
    iir = new IIRFilter;
    iir->setOrder(2);
    bypass = false;
    fir->setBypass(false);
    iir->setBypass(false);
    patch = new Patch();
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
    fir = new FIRFilter(*orig.fir);
    iir = new IIRFilter(*orig.iir);
}

BiquadFilter::~BiquadFilter() {
    delete patch;
    delete fir;
    delete iir;
}

bool BiquadFilter::process(){
    if(!bypass){
        iir->setBypass(false);
        fir->setBypass(false);
    }
    else{
        iir->setBypass(true);
        fir->setBypass(true);
    }
    bool result = iir->process();
    if(result)
        fir->process();
    return result;
}

bool BiquadFilter::addOutput(Patch* const patch){
    return fir->addOutput(patch);
}
bool BiquadFilter::addInput(Patch* const patch){
    return iir->addInput(patch);
}
bool BiquadFilter::removeOutput(Patch* const patch){
    return fir->removeOutput(patch);
}
bool BiquadFilter::removeInput(Patch* const patch){
    return iir->removeInput(patch);
}

void BiquadFilter::seta0(double coefficient){
    iir->setCoefficient(coefficient, 0);
}


void BiquadFilter::seta1(double coefficient){
    iir->setCoefficient(coefficient, 1);
}


void BiquadFilter::seta2(double coefficient){
    iir->setCoefficient(coefficient, 2);
}


void BiquadFilter::setb0(double coefficient){
    fir->setCoefficient(coefficient, 0);
}


void BiquadFilter::setb1(double coefficient){
    fir->setCoefficient(coefficient, 1);
}


void BiquadFilter::setb2(double coefficient){
    fir->setCoefficient(coefficient, 2);
}

Parameter& BiquadFilter::geta0() const{
    return iir->getCoefficient(0);
}
Parameter& BiquadFilter::geta1() const{
    return iir->getCoefficient(1);
}
Parameter& BiquadFilter::geta2() const{
    return iir->getCoefficient(2);
}

Parameter& BiquadFilter::getb0() const{
    return fir->getCoefficient(0);
}
Parameter& BiquadFilter::getb1() const{
    return fir->getCoefficient(1);
}
Parameter& BiquadFilter::getb2() const{
    return fir->getCoefficient(2);
}


//EOF