/* ************************************************************************
 * File:   Effect.h
 * Author: Nate Gallegos
 *
 * Log
 *      5/5/17
 *      Happy Cinco de Mayo!
 *      File Created
 ************************************************************************ */

#ifndef EFFECT_H
#define	EFFECT_H

#include "InputDevice.h"
#include "OutputDevice.h"
#include "Parameter.h"

/* *************************************************************************
 * Class: Effect
 * Purpose: To model a general interface for Effects
 ************************************************************************* */
class Effect : public InputDevice, public OutputDevice{
    
public:
    
    //Effect();
    
    virtual ~Effect(){bypass = false;}
    
    virtual bool process()=0;
    
    bool getBypass() const{return bypass;}
    
    void setBypass(bool bypass){
        this->bypass = bypass;
    }
    
protected:
    
    Parameter bypass;
};

#endif	/* EFFECT_H */

