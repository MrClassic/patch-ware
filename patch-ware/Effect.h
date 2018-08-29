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

/* *************************************************************************
 * Class: Effect
 * Purpose: To model a general interface for Effects
 ************************************************************************* */
class InputDevice;
class OutputDevice;
class Parameterizable;
class Effect : public InputDevice, public OutputDevice, public Parameterizable{
    
public:
    
	Effect() { params["bypass"] = false; };
    
    virtual ~Effect(){params["bypass"] = true;}
    
    virtual bool process()=0;
    
    bool getBypass() {return params["bypass"];}
    
    void setBypass(bool bypass){
        params["bypass"] = bypass;
    }
    

};

#endif	/* EFFECT_H */

