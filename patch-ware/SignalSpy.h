/* ************************************************************************
 * File:   SignalSpy.h
 * Author: Nate Gallegos
 *
 * Log
 *      3/4/18
 *      File Created
 ************************************************************************ */

#ifndef SIGNALSPY_H
#define	SIGNALSPY_H

#include "Effect.h"
#include <iostream>

class SignalSpy : public Effect{
    
public:
    SignalSpy(std::ostream &out){
        bypass = false;
        spy = &out;
    }
    
    SignalSpy(const SignalSpy &orig){
        //InputDevice::InputDevice(orig);
        //OutputDevice::OutputDevice(orig);
        bypass = orig.bypass;
        spy = orig.spy;
    }
    
    virtual bool process(){
        if(!*this){
            return false;
        }
        LinkedList<Patch> inputPatches = getInputPatches();
        while(!inputPatches.isEmpty()){
            Patch *pop = inputPatches.pop_front();
            if(pop != NULL){
                double signal = 0.;
                if(pop->requestSignal(signal)){;
                    pop->pushSignal(signal);
                    *spy << signal << ", ";
                }
            } 
        }
        *spy << '\n';
        output(input());
        return true;
    }
    
private:
    
    std::ostream* spy;
    
};

#endif	/* SIGNALSPY_H */

