/* ******************************************************************
 * File:   BiquadLowpass.h
 * Author: Nate Gallegos
 * 
 * Low pass filter implementation of a biquad filter.
 * cuts high frequencies and boosts the lows.
 * give extra THUMP to that bass drum or bass guitar...
 * or make that organ a little thicker...
 * or make the guitar *shine* more by eliminating 
 * low frequencies that muddy up the signal!
 * 
 * Log 
 *      10/25/18
 *		Created!
 ***************************************************************** */

#include "BiquadFilter.h"

#ifndef BIQUADLOWPASS_H
#define	BIQUADLOWPASS_H

//Low pass filter.
//All about that BASS!
class BiquadLowpass : public BiquadFilter {
public:
	BiquadLowpass();
	BiquadLowpass(const BiquadLowpass& orig);
    ~BiquadLowpass();
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