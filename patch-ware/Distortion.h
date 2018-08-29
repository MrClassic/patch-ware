/* ***********************************************************************
 * File:   Distortion.h
 * Author: Nate Gallegos
 *
 * Log
 *      5/5/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface
 */

#ifndef DISTORTION_H
#define	DISTORTION_H

#include "Effect.h"

/* ************************************************************************
 * Class: Distortion
 * Purpose: To model a distortion effect on an incoming audio signal.
 ************************************************************************ */
class Distortion : public Effect{
    
public:
    Distortion();
    Distortion(const Distortion& orig);
    ~Distortion();
    
    
    virtual bool process();
    
protected:
    
};

#endif	/* DISTORTION_H */

