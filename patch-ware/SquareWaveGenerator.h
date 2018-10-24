/* **********************************************************************
 * File:   SquareWaveGenerator.h
 * Author: Nate Gallegos
 *
 * Log
 *      4/26/17
 *      File Created
 */

#ifndef SQUAREWAVEGENERATOR_H
#define	SQUAREWAVEGENERATOR_H

#include"WaveProcessor.h"

/* **********************************************************************
 * Class: Square Wave Generator
 * Inherits From:   Wave Generator
 * Purpose: To emulate a Square Wave Generator. Easy right...?
 ********************************************************************** */
class SquareWaveGenerator : public WaveProcessor {
public:
    
    /* ********************************************************************
     *                      Public Section
     ******************************************************************** */
    
    /* *********************************************************************
     *                      Constructors
     ********************************************************************* */
    
    /* *********************************************************************
     * Default Constructor:
     ********************************************************************* */
    SquareWaveGenerator();
    
    /* *********************************************************************
     * Copy Constructor:
     ********************************************************************* */
    SquareWaveGenerator(const SquareWaveGenerator& orig);
    
    /* *********************************************************************
     * Constructor
     ********************************************************************* */
    SquareWaveGenerator(const double frenquency);
    
    /* *********************************************************************
     *                          Destructor
     ********************************************************************* */
    
    /* *********************************************************************
     * Destructor:
     ********************************************************************* */
    ~SquareWaveGenerator();
    
    /* *********************************************************************
     *                          Accessors
     ********************************************************************* */
    
    /* *********************************************************************
     * Get Double: 
     ********************************************************************* */
    double generate();
    
    void updateWaveOffset();
    
private:

    double waveOutput;
    bool firstHalf;
};

#endif	/* SQUAREWAVEGENERATOR_H */

//EOF