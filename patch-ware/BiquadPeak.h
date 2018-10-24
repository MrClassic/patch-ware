/* ******************************************************************
 * File:   BiquadPeak.h
 * Author: Nate Gallegos
 *
 * Log
 *      10/24/18
 *		Created!
 ***************************************************************** */

#include "BiquadFilter.h"

#ifndef BIQUADPEAK_H
#define	BIQUADPEAK_H

class BiquadPeak : public BiquadFilter {
public:
	BiquadPeak();
	BiquadPeak(const BiquadPeak& orig);
	~BiquadPeak();
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