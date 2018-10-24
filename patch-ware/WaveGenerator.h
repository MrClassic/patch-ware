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

#include "OutputDevice.h"
#include "Parameterizable.h"
#include "Timer.h"
#include "WaveProcessor.h"

class Parameter;

/* **************************************************************************
 * Virtual Class: Wave Generator
 * Purpose: This class is a prototype for a general wave generator. A wave
 * generator requires a feed of time intervals and a frequency to generate 
 * double values as displacement output to a speaker.
 ************************************************************************* */
class OutputDevice;
class Parameterizable;
class WaveGenerator : public OutputDevice, public Parameterizable, public Timer{
    
    /* ****************************************************************
     *                  Public Section
     ************************************************************** */
public:
    
	WaveGenerator(){
		proc = NULL;
	}

    /** ********************************************************************
     * Default Constructor
     * Initializes phase corrector and current time to zero (0).
     ******************************************************************** */
    WaveGenerator(WaveProcessor * const processor){
		proc = processor;
		addParameter("frequency", proc->paramAddr(WaveProcessor::FREQUENCY));
		addParameter("amplitude", proc->paramAddr(WaveProcessor::AMPLITUDE));
		addParameter("phase", proc->paramAddr(WaveProcessor::PHASE));
		params["frequency"] = 1.0;
		params["amplitude"] = 1.0;
		params["phase"] = 0.0;

	}
    
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
	virtual bool process() {
		if (!paramsReady()) {
			return false;
		}
		if (proc != NULL) {
			proc->process();
			return true;
		}
		return false;
	}
    
    /* *****************************************************************
     * Get Frequency: Accessor for this wave generator's frequency parameter. 
     * Returns the actual address of the frequency parameter. Use with caution.
     * Pre-condition:   The frequency parameter should be initialized.
     * Post-condition:  The address of the frequency parameter owned by this 
     * wave generator.
     ****************************************************************** */
    Parameter& getFrequency(){
		return params["frequency"];
    }
    
    /* *****************************************************************
     * Get Amplitude: Accessor for this wave generator's amplitude parameter. 
     * Returns the actual address of the amplitude parameter. Use with caution.
     * Pre-condition:   The amplitude parameter should be initialized.
     * Post-condition:  The address of the amplitude parameter owned by this 
     * wave generator.
     ****************************************************************** */
    Parameter& getAmplitude(){
        return params["amplitude"];
    }
    
    /* *****************************************************************
     * Get Phase: Accessor for this wave generator's phase parameter. 
     * Returns the actual address of the phase parameter. Use with caution.
     * Pre-condition:   The phase parameter should be initialized.
     * Post-condition:  The address of the phase parameter owned by this 
     * wave generator.
     ****************************************************************** */
    Parameter& getPhase() {
        return params["phase"];
    }

	void incrementTime(const double time) {
		proc->frameRate = time;
	}

	bool setParameter(const std::string &tag, const double value) {
		if (tag == "frequency") {
			setFrequency(value);
		}
		else if (tag == "phase") {
			setPhase(value);
		}
		else if (tag == "amplitude") {
			setAmplitude(value);
		}
		else {
			return Parameterizable::setParameter(tag, value);
		}
		return true;
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
    virtual bool setFrequency(const double frequency){
		return params["frequency"].setParameter(frequency);
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
    virtual bool setAmplitude(const double amplitude){
		return params["amplitude"].setParameter(amplitude);
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
    virtual bool setPhase(const double phase){
		return params["phase"].setParameter(phase);
    };
    
    /* ********************************************************************
     * Increment Time: Increments the current time variable to update the wave
     * generator of how much time has elapsed per frequency periods.
     * Pre-condition:   The time parameter should be initialized.
     * Post-condition:  The current time variable will be updated.
     ********************************************************************* 
    virtual void incrementTime(const double time){
        currentTime += time;
		const double freq = params["frequency"];
        while(currentTime > (1. / freq)){
            currentTime -= (1. / freq);
        }
        while(currentTime < 0){
            currentTime += (1. / freq);
        }
    }
	!!!! probably wont need !!!!
	*/
    
    /* ********************************************************************
     * Update Wave Offset [pure virtual]: Calculates the phase corrector
     * variable. The process of doing this may depend on the wave being 
     * implemented, so the this function is left pure virtual.
     ******************************************************************** */
	void updateWaveOffset() {
		if (proc != NULL) {
			proc->updateWaveOffset();
		}
	}

	/*
	Accessor for this WaveGenerator's processor
	*/
	WaveProcessor* getProc() { return proc; }
    
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
		if (proc == NULL)
			return 0.;
		return proc->getPhaseOffset();
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
    bool paramsReady() {
		return parametersReady();
    }
    
	//boolean cast for checking ready state
	operator bool() {
		return parametersReady();
	}

    /* ********************************************************************
     *                          Protected Section
     ******************************************************************** */
    
protected:
    
	WaveProcessor *proc;

};

#endif	/* WAVEGENERATOR_H */

//EOF