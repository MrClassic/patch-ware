/* ***********************************************************************
 * File:   Distortion.h
 * Author: Nate Gallegos
 *
 * Log
 *      5/5/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface
 *		10/8/18
 *		Changed to implement the SignalProcessing Interface
 */

#ifndef DISTORTION_H
#define	DISTORTION_H

#include "pwmath.h"
#include "SignalProcessor.h"

/* ************************************************************************
 * Class: Distortion
 * Purpose: To model a distortion effect on an incoming audio signal.
 ************************************************************************ */
class Distortion : public SignalProcessor{
    
public:
    Distortion();
    Distortion(const Distortion& orig);
    ~Distortion();
    
    
    double processSignal(const double &);
    
	enum parameter {
		BYPASS = 0,
		THRESH,
		NUM_PARAMS
	};

protected:
    
	
};

#endif	/* DISTORTION_H */

