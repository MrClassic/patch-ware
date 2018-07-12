/* **************************************************************************
 * File:   InvertDistortion.h
 * Author: Nate Gallegos
 *
 * Log
 *      5/5/17
 *      File Created
 ************************************************************************** */

#ifndef INVERTDISTORTION_H
#define	INVERTDISTORTION_H

#include "Distortion.h"

class InvertDistortion : public Distortion{

public:
        
    InvertDistortion(){threshold = 1.0;}
    
    ~InvertDistortion(){}
    
    bool process(){
        if(!*this || (threshold.getInputCount() > 0 && !threshold.isReady())){
            return false;
        }
        double signal = input();
        if(signal > 0. && signal > (double)threshold && !bypass){
            output(threshold - signal + (double)threshold);
        }
        else if(signal < 0. && signal < (threshold * -1.) && !bypass){
            output(threshold * -1. - signal - (double)threshold);
        }
        else{
            output(signal);
        }
        return true;
    }
};

#endif	/* INVERTDISTORTION_H */

