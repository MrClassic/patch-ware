/* **********************************************************************
 * File:   Gain.h
 * Author: Nate Gallegos
 * 
 * Log
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface
 ********************************************************************* */

#ifndef GAIN_H
#define	GAIN_H

#include "SignalProcessor.h"

class Gain : public SignalProcessor{
public:
    Gain();
    Gain(const Gain& orig);
    virtual ~Gain();
    
    virtual double processSignal(const double &);
    
	enum parameter {
		BYPASS = 0,
		LEVEL,
		NUM_PARAMS
	};

private:

};

#endif	/* GAIN_H */

