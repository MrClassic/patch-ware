/* **************************************************************************
 * File:   InvertDistortion.h
 * Author: Nate Gallegos
 *
 * Log
 *      5/5/17
 *      File Created
 *		8/15/18
 *		implemented Parameterizable interface
 ************************************************************************** */

#ifndef INVERTDISTORTION_H
#define	INVERTDISTORTION_H

#include "Distortion.h"

class InvertDistortion : public Distortion{

public:
        
    InvertDistortion() : Distortion() {/* Do nothing */}

	InvertDistortion(const InvertDistortion &orig) : Distortion(orig) { /* do nothing */ }

    virtual ~InvertDistortion(){}
    
    virtual double processSignal(const double &signal){
		
		if (params[BYPASS] >= 1.)
			return signal;

		//run invert distortion algorithm
        if(signal > 0. && signal > (double)params[THRESH]){
            return params[THRESH] - signal + params[THRESH];
        }
        else if(signal < 0. && signal < params[THRESH] * -1.){
            return params[THRESH] * -1. - signal - (double)params[THRESH];
        }
        else{
            return signal;
        }

    }
};

#endif	/* INVERTDISTORTION_H */

