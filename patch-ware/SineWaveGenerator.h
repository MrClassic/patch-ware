/* *****************************************************************
 * File:   SineWaveGenerator.h
 * Author: Nate Gallegos
 *
 * Log
 *      4/27/17
 *      File Created
 *      basic declarations for virtual functions
 ***************************************************************** */

#ifndef SINEWAVEGENERATOR_H
#define	SINEWAVEGENERATOR_H

#include "sineTable.h"
#include "WaveGenerator.h"

class SineWaveGenerator : public WaveGenerator{
    /* ********************************************************************
     *                      Public Section
     ******************************************************************** */
public:
    /* *********************************************************************
     *                      Constructors
     ********************************************************************* */
    
    /* *********************************************************************
     * Default Constructor:
     ********************************************************************* */
    SineWaveGenerator();
    
    /* *********************************************************************
     * Constructor
     ********************************************************************* */
    SineWaveGenerator(double frenquency);
    
    /* *********************************************************************
     * Copy Constructor:
     ********************************************************************* */
    SineWaveGenerator(const SineWaveGenerator& orig);
    
    /* *********************************************************************
     * set Frequency:
     ********************************************************************* */
    //void setFrequency(double freq);
    
    void updateWaveOffset();
    
    /* *********************************************************************
     *                          Destructor
     ********************************************************************* */
    
    /* *********************************************************************
     * Destructor:
     ********************************************************************* */
    ~SineWaveGenerator();
    
    /* *********************************************************************
     *                          Accessors
     ********************************************************************* */
    
    /* *********************************************************************
     * Get Double: 
     ********************************************************************* */
    bool process();
    
private:
    //double amplitudeEnvelope;
    //double amplitudeEnvelopeStartVal;
    //double amplitudeEnvelopeStartTime;
};

#endif	/* SINEWAVEGENERATOR_H */

