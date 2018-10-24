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

#include "SignalProcessor.h"
#include "Circular.h"


class Delay : public SignalProcessor{
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
    
    double processSignal(const double &);
    
	enum parameter {
		BYPASS = 0,
		DECAY,
		REGEN,
		MIX,
		NUM_PARAMS
	};
    
protected:

	void setMemory(int blocks);

    circular_queue<double> registers;
};

#endif	/* DELAY_H */

