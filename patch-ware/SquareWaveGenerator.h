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

#include"WaveGenerator.h"

/* **********************************************************************
 * Class: Square Wave Generator
 * Inherits From:   Wave Generator
 * Purpose: To emulate a Square Wave Generator. Easy right...?
 ********************************************************************** */
class SquareWaveGenerator : public WaveGenerator{
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
    bool pushDouble() const;
    
private:

};

#endif	/* SQUAREWAVEGENERATOR_H */

//EOF