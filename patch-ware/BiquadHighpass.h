/* ******************************************************************
 * File:   BiquadHighpass.h
 * Author: Nate Gallegos
 * 
 * High pass filter implementation of the BiquadFilter.
 * Trims low frequencies and boosts higher frequencies.
 * Make those cymbals shine or cut those nasty inaudible
 * low frequencies your microphone picked up!
 * 
 * Log 
 *      10/25/18
 *		Created!
 ***************************************************************** */

#include "BiquadFilter.h"

#ifndef BIQUADHIGHPASS_H
#define	BIQUADHIGHPASS_H

//Highpass filter, cut the bass, boost the high!
class BiquadHighpass : public BiquadFilter {
public:
	BiquadHighpass();
	BiquadHighpass(const BiquadHighpass& orig);
    ~BiquadHighpass();
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