/* ************************************************************************
 * File:   Compressor.h
 * Author: Nate Gallegos
 * 
 * Log
 *      5/4/17
 *      File Created
 *		8/15/18
 *		implemented Para,eterizable interface
 *********************************************************************** */


#ifndef COMPRESSOR_H
#define	COMPRESSOR_H

#include "SignalProcessor.h"

/* ************************************************************************
 * Class:   Compressor
 * Purpose: To model a compressor for audio. The Compressor will take in double
 * values as an input signal. Any signal value greater than the buffer value 
 * will be compressed to max out at the threshold value. All input signals are
 * boosted such that the threshold maximum value for a compressed sig nal will
 * output as the maximum value for the signal.
 ************************************************************************** */
class Compressor : public SignalProcessor{
    
    /* **********************************************************************
     * 
     *                          Public Section
     * 
     ********************************************************************** */
public:
    
    /* **********************************************************************
     *                          Constructors
     ********************************************************************** */
    Compressor();
    Compressor(const Compressor& orig);
    
    /* **********************************************************************
     *                          Destructor
     ********************************************************************** */
    ~Compressor();
    
    /* **********************************************************************
     *                          Accessors
     ********************************************************************** */
    double processSignal(const double &);
    
    
    /* **********************************************************************
     *                          Mutators
     ********************************************************************** */
    
	enum parameter {
		BYPASS = 0,
		MAX,
		THRESH,
		BUFFER,
		NUM_PARAMS
	};
    
protected:
    
    
};

#endif	/* COMPRESSOR_H */

