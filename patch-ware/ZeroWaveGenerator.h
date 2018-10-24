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

class ZeroWaveGenerator : public WaveProcessor{
    
public:
        
    ZeroWaveGenerator();
    ZeroWaveGenerator(double amplitude);
    ZeroWaveGenerator(const ZeroWaveGenerator &orig);
    
    ~ZeroWaveGenerator();
    
    void updateWaveOffset();
    
    double generate();
};

#endif	/* ZEROWAVEGENERATOR_H */

