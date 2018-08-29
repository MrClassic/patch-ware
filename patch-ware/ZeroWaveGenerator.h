/* *************************************************************************
 * File:   ZeroWaveGenerator.h
 * Author: Nate Gallegos
 *
 * Log
 *      2/3/18
 *      File Created
 *          
 ************************************************************************* */
 

#ifndef ZEROWAVEGENERATOR_H
#define	ZEROWAVEGENERATOR_H

#include "WaveGenerator.h"

class ZeroWaveGenerator : public WaveGenerator{
    
public:
        
    ZeroWaveGenerator();
    ZeroWaveGenerator(double amplitude);
    ZeroWaveGenerator(const ZeroWaveGenerator &orig);
    
    ~ZeroWaveGenerator();
    
    void updateWaveOffset();
    
    bool process();
};

#endif	/* ZEROWAVEGENERATOR_H */

