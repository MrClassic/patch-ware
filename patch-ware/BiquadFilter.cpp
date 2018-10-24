/* ******************************************************************
 * File:   BiquadFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface through Effect
 *		10/8/18
 *		changed to implement the SignalProcessor interface
 ***************************************************************** */

#include "BiquadFilter.h"

BiquadFilter::BiquadFilter() : SignalProcessor() {

	params.resize(NUM_PARAMS);

	//make fir half
    fir = new FIRFilter;
    fir->setOrder(2);
	fir->type = SUM;
	
	//make iir half
    iir = new IIRFilter;
    iir->setOrder(2);
	iir->type = SUM;

}

BiquadFilter::BiquadFilter(const BiquadFilter& orig) : SignalProcessor(orig) {
    if(fir != NULL){
        delete fir;
    }
    if(iir != NULL){
        delete iir;
    }

	//shallow copy
    fir = new FIRFilter(*orig.fir);
    iir = new IIRFilter(*orig.iir);

}

BiquadFilter::~BiquadFilter() {

	//clean up your mess!!!
    delete fir;
    delete iir;
}

double BiquadFilter::processSignal(const double &signal){
	//updated bypass status for internal filters
    if(params[BYPASS] < 1.){
		iir->params[BYPASS] = 0.;
        fir->params[BYPASS] = 0.;
    }
    else{
        iir->params[BYPASS] = 1.;
        fir->params[BYPASS] = 1.;
    }
	// [signal] >>>> input >>>> fir >>>> iir >>>> output >>>>
	return iir->processSignal(fir->processSignal(signal));
    
}

//EOF