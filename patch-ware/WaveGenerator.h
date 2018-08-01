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
 *      6/30/18
 *      Updated documentation.
 *		7/28/18
 *		Made incrementTime(double) virtual for
 *			inheritance into PulseGenerator
 *          
 ************************************************************************* */

#ifndef WAVEGENERATOR_H
#define	WAVEGENERATOR_H

#include "Parameter.h"

class Parameter;

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
    
    /** ********************************************************************
     * Default Constructor
     * Initializes phase corrector and current time to zero (0).
     ******************************************************************** */
    WaveGenerator(){phaseCorrecter = 0.;currentTime = 0.;}
    
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
    virtual bool pushDouble()=0;
    
    /* *****************************************************************
     * Get Frequency: Accessor for this wave generator's frequency parameter. 
     * Returns the actual address of the frequency parameter. Use with caution.
     * Pre-condition:   The frequency parameter should be initialized.
     * Post-condition:  The address of the frequency parameter owned by this 
     * wave generator.
     ****************************************************************** */
    Parameter& getFrequency(){
        return frequency;
    }
    
    /* *****************************************************************
     * Get Amplitude: Accessor for this wave generator's amplitude parameter. 
     * Returns the actual address of the amplitude parameter. Use with caution.
     * Pre-condition:   The amplitude parameter should be initialized.
     * Post-condition:  The address of the amplitude parameter owned by this 
     * wave generator.
     ****************************************************************** */
    Parameter& getAmplitude(){
        return amplitude;
    }
    
    /* *****************************************************************
     * Get Phase: Accessor for this wave generator's phase parameter. 
     * Returns the actual address of the phase parameter. Use with caution.
     * Pre-condition:   The phase parameter should be initialized.
     * Post-condition:  The address of the phase parameter owned by this 
     * wave generator.
     ****************************************************************** */
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
    virtual void setFrequency(const double frequency){
        double t = currentTime;
        lastFrequency = (double)(this->frequency); 
        this->frequency.setParameter(frequency);
        currentTime = t;
    };
    
    /* *********************************************************************
     * Set Amplitude: Sets the maximum displacement from equilibrium as the 
     * amplitude member of this WaveGenerator.
     * Note: the amplitude can be negative but is not enforced, please ensure
     * that the equations being implemented with the amplitude handle these
     * cases appropriately.
     * Pre-condition:   The parameter should be initialized.
     * Post-condition:  The amplitude for the Wave Generator will be set.
     ********************************************************************** */
    virtual void setAmplitude(const double amplitude){
        lastAmplitude = (double)this->amplitude;
        this->amplitude.setParameter(amplitude);
    };
    
    /* ********************************************************************
     * Set Phase: Sets the phase of the wave being created by this wave 
     * generator. Phase is considered continuous for the purpose of this 
     * wave generator and is not handled with any special cases, unlike 
     * changes in frequency and amplitude.
     * Pre-condition:   The parameter should be initialized
     * Post-condition:  The phase and lastPhase of the wave generator will be 
     * updated.
     ******************************************************************** */
    virtual void setPhase(const double phase){
        lastPhase = (double)this->phase;
        this->phase.setParameter(phase);
    };
    
    /* ********************************************************************
     * Increment Time: Increments the current time variable to update the wave
     * generator of how much time has elapsed per frequency periods.
     * Pre-condition:   The time parameter should be initialized.
     * Post-condition:  The current time variable will be updated.
     ********************************************************************* */
    virtual void incrementTime(const double time){
        currentTime += time;
        while(currentTime > (1. / (double)frequency)){
            currentTime -= (1. / (double)frequency);
        }
        while(currentTime < 0){
            currentTime += (1. / (double)frequency);
        }
    }
    
    /* ********************************************************************
     * Update Wave Offset [pure virtual]: Calculates the phase corrector
     * variable. The process of doing this may depend on the wave being 
     * implemented, so the this function is left pure virtual.
     ******************************************************************** */
    virtual void updateWaveOffset()=0;
    
    /* ********************************************************************
     * Get Phase Offset: Calculates and returns the effective wave phase 
     * based on the actual phase of the wave and the phase offset. Calls
     * updateWaveOffset which is virtual and defined by the specific 
     * implementing class of the wave generator virtual class.
     * Pre-condition:   The frequency and phase parameters should be 
     * initialized.
     * Post-condition:  The effective phase is returned.
     ******************************************************************** */
    double getPhaseOffset() {
        
        if((double)frequency == 0){
            return 0;
        }
        
        updateWaveOffset();
        double out = (phase + phaseCorrecter);
        while(out > (1)){
            out -= (1);
        }
        while(out < 0){
            out += (1);
        }
        return out;
    }
    
    /* ********************************************************************
     * Params Ready: Checks for the three parameters (amplitude, frequency and
     * phase) to be ready. Returns true if they are ready for input, false 
     * if any are not.
     * Pre-condition:   The three parameters (amplitude, phase and frequency)
     * should be initialized.
     * Post-condition:  The result is returned, true if all parameters are 
     * ready, false otherwise.
     ******************************************************************** */
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
    double lastFrequency;
    double lastAmplitude;
    double lastPhase;
    double phaseCorrecter; //double for phase correction when switching
                            //frequencies
    //double amplitudeEnvelope;
};

#endif	/* WAVEGENERATOR_H */

//EOF