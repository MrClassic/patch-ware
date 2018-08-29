/* **********************************************************************
 * File:   Gain.h
 * Author: Nate Gallegos
 * 
 * Log
 *      5/11/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface
 ********************************************************************* */

#ifndef GAIN_H
#define	GAIN_H

#include "Effect.h"

class Gain : public Effect{
public:
    Gain();
    Gain(const Gain& orig);
    virtual ~Gain();
    
    virtual bool process();
    
private:

};

#endif	/* GAIN_H */

