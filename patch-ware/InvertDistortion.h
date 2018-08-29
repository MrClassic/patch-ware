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
        
    InvertDistortion(){/* Do nothing */}

	InvertDistortion(const InvertDistortion &orig) { copyParameters(orig); }

    virtual ~InvertDistortion(){}
    
    virtual bool process(){
		//check for ready state
        if(!isReady() || (params["threshold"].isPatched() && !params["threshold"].isReady())){
            return false;
        }
		//get input signal
        double signal = input();

		//run invert distortion algorithm
        if(signal > 0. && signal > (double)params["threshold"] && !params["bypass"]){
            output((double)params["threshold"] - signal + (double)params["threshold"]);
        }
        else if(signal < 0. && signal < (params["threshold"] * -1.) && !params["bypass"]){
            output((double)params["threshold"] * -1. - signal - (double)params["threshold"]);
        }
        else{
            output(signal);
        }

		//success!
        return true;
    }
};

#endif	/* INVERTDISTORTION_H */

