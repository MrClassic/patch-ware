/* *****************************************************************
 * File:   QuadraticWaveGenerator.h
 * Author: Nate Gallegos
 *
 * Log
 *      4/30/17
 *      File Created
 *      basic declarations for virtual functions
 ***************************************************************** */

#ifndef QUADRACTICWAVEGENERATOR_H
#define	QUADRACTICWAVEGENERATOR_H

#include<cmath>
#include "WaveGenerator.h"

class QuadraticWaveGenerator : public WaveGenerator{
    /* ********************************************************************
     *                      Public Section
     ******************************************************************** */
public:
    /* *********************************************************************
     *                      Constructors
     ********************************************************************* */
    
    /* *********************************************************************
     * Default Constructor:
     ********************************************************************* */
    QuadraticWaveGenerator();
    
    /* *********************************************************************
     * Copy Constructor:
     ********************************************************************* */
    QuadraticWaveGenerator(const QuadraticWaveGenerator &orig);
    
    /* *********************************************************************
     * Constructor
     ********************************************************************* */
    QuadraticWaveGenerator(double frenquency);
    
    /* *********************************************************************
     *                          Destructor
     ********************************************************************* */
    
    /* *********************************************************************
     * Destructor:
     ********************************************************************* */
    ~QuadraticWaveGenerator();
    
    /* *********************************************************************
     *                          Accessors
     ********************************************************************* */
    
    /* *********************************************************************
     * Push Double: 
     ********************************************************************* */
    bool process();
    
private:

};

#endif	/* QUADRACTICWAVEGENERATOR_H */

