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
	fir->type = AVERAGE;
	
	//make iir half
    iir = new IIRFilter;
    iir->setOrder(2);
	iir->type = AVERAGE;

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

	//set internal filters' coefficients
	fir->params[FIRFilter::COEFFICIENTS] = params[A0];
	fir->params[FIRFilter::COEFFICIENTS + 1] = params[A1];
	fir->params[FIRFilter::COEFFICIENTS + 2] = params[A2];

	iir->params[IIRFilter::COEFFICIENTS] = params[B0];
	iir->params[IIRFilter::COEFFICIENTS + 1] = params[B1];
	iir->params[IIRFilter::COEFFICIENTS + 2] = params[B2];

	// [signal] >>>> input >>>> fir >>>> iir >>>> output >>>>
	//bool neg = (signal < 0);
	//double sig = signal;
	
	//sig = log(abs(sig))*20.;
	double out = iir->processSignal(fir->processSignal(signal));
	//if (abs(out) > 1.) {
	//	for (int i = 0; i < 2; i++) {
	//		iir->registers[i] = 0.;
	//		fir->registers[i] = 0.;
	//	}
	//}
	return out;
	//sig = pow(10., (sig / 20.));
	//if (neg) sig *= -1.;
	//return sig;
}

//EOF