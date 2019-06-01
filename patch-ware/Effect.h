/* ************************************************************************
 * File:   Effect.h
 * Author: Nate Gallegos
 *
 * Log
 *      5/5/17
 *      Happy Cinco de Mayo!
 *      File Created
 *		8/14/18
 *		implemented the Parameterizable interface
 ************************************************************************ */

#ifndef EFFECT_H
#define	EFFECT_H

#include "Parameterizable.h"
#include "SignalProcessor.h"

/* *************************************************************************
 * Class: Effect
 * Purpose: To model a general interface for Effects
 ************************************************************************* */
class InputDevice;
class OutputDevice;
class Parameterizable;
class Effect : public InputDevice, public OutputDevice, public Parameterizable{
    
protected:

	SignalProcessor* proc;

public:
    
	Effect() { /*addParameter("bypass"); params["bypass"] = false;*/ proc = NULL; };

	Effect(SignalProcessor* signalProcessor) : Effect() { 
		proc = signalProcessor; 
		addParameter("bypass", proc->paramAddr(0)); //0 is BYPASS param index
		params["bypass"] = false;
	}

	SignalProcessor* getProc() { return proc; }

	void setProc(SignalProcessor* sigProc) { proc = sigProc; }

	void setInputType(input_type in) {
		inputType = in;
		proc->type = in;
	}
    
	virtual ~Effect() { /* do nothing */ }
    
	virtual bool process() {

		//validate
		if (proc == NULL || !checkInputs() || !parametersReady()) {
			return false;
		}

		//update parameters
		updateParameters();

		//get input signal (condense inputs)
		double signal = input();

		//process using Signal Processor
		signal = proc->processSignal(signal);

		//send result to outputs
		output(signal);

		return true;
	};
    
    bool getBypass() {return params["bypass"];}
    
    void setBypass(bool bypass){
        params["bypass"] = bypass;
    }
    


};

#endif	/* EFFECT_H */

