/* **********************************************************************
 * File:   Gain.h
 * Author: Nate Gallegos
 * 
 * Log
 *      5/11/17
 *      File Created
 ********************************************************************* */

#ifndef GAIN_H
#define	GAIN_H

#include "Effect.h"

class Gain : public Effect{
public:
    Gain();
    Gain(const Gain& orig);
    ~Gain();
    
    Parameter& getLevel();
    
    void setLevel(double level);
    bool process();
    
private:

    Parameter level;
};

#endif	/* GAIN_H */

