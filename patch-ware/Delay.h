/* ************************************************************************
 * File:   Delay.h
 * Author: Nate Gallegos
 * 
 * Log 
 *      5/5/17
 *      File Created
 *********************************************************************** */

#ifndef NULL
#define NULL 0x0
#endif

#ifndef DELAY_H
#define	DELAY_H

#include "Effect.h"
#include "Circular.h"


class Delay : public Effect{
public:
    
    /* **********************************************************************
     *                          Constructors
     ********************************************************************** */
    Delay();
    
    Delay(const Delay& orig);
    
    /* **********************************************************************
     *                          Destructor
     ********************************************************************** */
    
    ~Delay();
    
    /* **********************************************************************
     *                          Accessors
     ********************************************************************** */
    
    Parameter& getDecay();
    
    /* **********************************************************************
     *                          Mutators
     ********************************************************************** */
    
    bool process();
    
    void setDecay(double decay);
    
    void setMemory(int blocks);
    
protected:
    
    int registerSize;
    Parameter decay;
    
    circular_queue<double> registers;
};

#endif	/* DELAY_H */

