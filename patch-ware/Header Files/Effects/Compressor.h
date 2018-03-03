/* ************************************************************************
 * File:   Compressor.h
 * Author: Nate Gallegos
 * 
 * Log
 *      5/4/17
 *      File Created
 *********************************************************************** */


#ifndef COMPRESSOR_H
#define	COMPRESSOR_H

#include "Effect.h"

/* ************************************************************************
 * Class:   Compressor
 * Purpose: To model a compressor for audio. The Compressor will take in double
 * values as an input signal. Any signal value greater than the buffer value 
 * will be compressed to max out at the threshold value. All input signals are
 * boosted such that the threshold maximum value for a compressed sig nal will
 * output as the maximum value for the signal.
 ************************************************************************** */
class Compressor : public Effect{
    
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
    bool process();
    Parameter& getThreshold();
    Parameter& getBuffer();
    Parameter& getMax();
    
    /* **********************************************************************
     *                          Mutators
     ********************************************************************** */
    void setThreshold(double threshold);
    void setBuffer(double buffer);
    void setMax(double max);
    
protected:
    Parameter threshold;   //threshold
    Parameter buffer;      //buffer zone start
    Parameter max;         //max output
    
};

#endif	/* COMPRESSOR_H */

