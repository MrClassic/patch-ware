/* ************************************************************************
 * File:   SignalSpy.h
 * Author: Nate Gallegos
 *
 * Log
 *      3/4/18
 *      File Created
 *		7/15/18
 *		Updated documentation
 *		8/15/18
 *		implemented Parameterizable interface
 ************************************************************************ */

#ifndef SIGNALSPY_H
#define	SIGNALSPY_H

#include "Effect.h"
#include <iostream>

/* ***********************************************************************
 * Class: Signal Spy
 * Purpose: To provide a way to output a signal to a file or to the console.
 * This class uses an ostream object to output the signal it receives form
 * its input channel(s) and acts as a signal bypass, passing the input
 * signal to its output channel(s).
 ************************************************************************ */
class SignalSpy : public Effect{
    
public:
	/*
	Initialization Constructor: Creates a new SignalSpy, and initializes
	its ostream for signal outputting to the ostream provided.
	*/
    SignalSpy(std::ostream &out){
        spy = &out;
    }
    
	/*
	Copy Constructor: Creates a copy of the parameterized SignalSpy.
	Only copies the bypass state and the ostream reference. The newly created
	SignalSpy will have no input or output channels initialized.
	*/
    SignalSpy(const SignalSpy &orig){
		copyParameters(orig);
        spy = orig.spy;
    }
    
	/*
	Process: Attempts to process the input channels by writing them to the
	ostream, and outputting the input signal(s) to the output channel(s).
	*/
    virtual bool process(){
        if(!isReady()){
            return false;
        }
        LinkedList<Patch> inputPatches = getInputPatches();
        while(!inputPatches.isEmpty()){
            Patch *pop = inputPatches.pop_front();
            if(pop != NULL){
                double signal = 0.;
				//take signal out of patch
                if(pop->requestSignal(signal)){
					//put signal back into patch
                    pop->pushSignal(signal);
					//send signal to ostream
                    *spy << signal << ", ";
                }
            } 
        }
        *spy << '\n';

		//send input straight through to output
        output(input());

		//success!
        return true;
    }
    
private:
    
	//signal output ostream
    std::ostream* spy;
    
};

#endif	/* SIGNALSPY_H */

