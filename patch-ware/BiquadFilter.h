/* ******************************************************************
 * File:   BiquadFilter.h
 * Author: Nate Gallegos
 * 
 * Standard, Open-ended biquad filter. Use as is for a challenge
 * (or if you are a DSP guru), or use one of the sub classes that
 * shape the frequency response for you... Your call!
 * 
 * Log 
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface through Effect
 ***************************************************************** */

#include "FIRFilter.h"
#include "IIRFilter.h"

#ifndef BIQUADFILTER_H
#define	BIQUADFILTER_H

class BiquadFilter : public SignalProcessor {
public:
    BiquadFilter();
    BiquadFilter(const BiquadFilter& orig);
    virtual ~BiquadFilter();
    virtual double processSignal(const double &signal);
    
	enum parameters {
		BYPASS = 0,
		A0,
		A1,
		A2,
		B0,
		B1,
		B2,
		NUM_PARAMS
	};
private:

    FIRFilter* fir;
    IIRFilter* iir;

};

#endif	/* BIQUADFILTER_H */

//EOF