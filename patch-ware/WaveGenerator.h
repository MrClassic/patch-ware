/* *************************************************************************
 * File:   WaveGenerator.h
 * Author: Nate Gallegos
 *
 * Log
 *      4/26/17
 *      File Created
 *      declared virtual functions:
 *          destructor
 *          getDouble()
 *          setFrequency(double)
 *      declared protected members:
 *          double currentTime
 *          double frequency
 *      2/28/18
 *      Implements OutputDevice.
 *          
 ************************************************************************* */

#ifndef WAVEGENERATOR_H
#define	WAVEGENERATOR_H

#include "Patch.h"
#include "Parameter.h"

/* **************************************************************************
 * Virtual Class: Wave Generator
 * Purpose: This class is a prototype for a general wave generator. A wave
 * generator requires a feed of time intervals and a frequency to generate 
 * double values as displacement output to a speaker.
 ************************************************************************* */
class WaveGenerator : public OutputDevice {
    
    /* ****************************************************************
     *                  Public Section
     ************************************************************** */
public:
    WaveGenerator(){}
    /* **************************************************************
     * Destructor: Enables a WaveGenerator pointer to call it's derived class'
     * destructor.
     * Pre-condition:   None
     * Post-condition:  The derived class' destructor will be called instead of
     *                  the WaveGenerator's automatically assigned destructor.
     ***************************************************************** */
    virtual ~WaveGenerator(){};
    
    /* *****************************************************************
     * Push Double:  pushes a double value representing the pressure
     * displacement from equilibrium at the current time of this wave
     * generator to the output Patches.
     * Pre-condition:   The current time should be incremented between calls
     *                  and the frequency should be set.
     * Post-condition:  A double value is returned representing the displacement
     *                  of the wave at its current time.
     ******************************************************************* */
    virtual bool pushDouble() const=0;
    
    Parameter& getFrequency(){
        return frequency;
    }
    
    Parameter& getAmplitude(){
        return amplitude;
    }
    
    Parameter& getPhase() {
        return phase;
    }
    /* *********************************************************************
     * Set Frequency: Sets the Wave Generator's frequency.
     * [NOTE] the frequency is set as CYCLIC FREQUENCY for ease of use.
     * How the derived class handles a change in frequency is up to the 
     * designer of that class.
     * Pre-condition:   The frequency parameter should be initialized and should
     *                  be larger than 0.
     * Post-condition:  The frequency for the Wave Generator will be set.
     ********************************************************************** */
    virtual void setFrequency(const double frequency){this->frequency = frequency; currentTime = 0.0;};
    
    /* *********************************************************************
     * Set Amplitude: Sets the maximum displacement from equilibrium as the 
     * amplitude member of this WaveGenerator.
     * Note: the amplitude can be negative but is not enforced, please ensure
     * that the equations being implemented with the amplitude handle these
     * cases appropriately.
     * Pre-condition:   The parameter should be initialized.
     * Post-condition:  The amplitude for the Wave Generator will be set.
     ********************************************************************** */
    virtual void setAmplitude(const double amplitude){this->amplitude = amplitude;};
    
    virtual void setPhase(const double phase){this->phase = phase;};
    
    /* ********************************************************************
     * Increment Time: Increments the current time variable to update the wave
     * generator of how much time has elapsed per frequency periods.
     * Pre-condition:   The time parameter should be initialized.
     * Post-condition:  The current time variable will be updated.
     ********************************************************************* */
    void incrementTime(const double time){
        currentTime += time;
        while(currentTime > (1/(double)frequency)){
            currentTime -= (1/(double)frequency);
        }
    }
    
    bool paramsReady() const{
        if (amplitude.getInputCount() > 0 && !amplitude.isReady()) {
            return false;
        }
        if (phase.getInputCount() > 0 && !phase.isReady()) {
            return false;
        }
        if (frequency.getInputCount() > 0 && !frequency.isReady()) {
            return false;
        }
        return true;
    }
    
    /* ********************************************************************
     *                          Protected Section
     ******************************************************************** */
    
protected:
    
    //member variables
    double currentTime; //current time between 0 and frequency
    Parameter frequency;   //cyclic frequency
    Parameter amplitude;   //wave amplitude
    Parameter phase;       //wave phase offest
};

#endif	/* WAVEGENERATOR_H */

//EOF