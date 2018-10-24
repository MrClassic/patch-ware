/* ******************************************************************
 * File:   BiquadBandpass.h
 * Author: Nate Gallegos
 * 
 * Log 
 *      10/24/18
 *		Created!
 ***************************************************************** */

#include "BiquadFilter.h"

#ifndef BIQUADBANDPASS_H
#define	BIQUADBANDPASS_H

class BiquadBandpass : public BiquadFilter {
public:
	BiquadBandpass();
	BiquadBandpass(const BiquadBandpass& orig);
    ~BiquadBandpass();
    double processSignal(const double &signal);
    
	enum parameters {
		BYPASS = 0,
		A0,
		A1,
		A2,
		B0,
		B1,
		B2,
		FC,
		Q,
		PEAK,
		NUM_PARAMS
	};
private:

	void updateCoefficients();
	double lastFC, lastQ, lastPeak;
};

#endif	/* BIQUADFILTER_H */

//EOF