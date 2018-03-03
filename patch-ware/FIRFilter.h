/* ************************************************************************
 * File:   FIRFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/3/17
 *      File Created
 ************************************************************************ */

#ifndef FIRFILTER_H
#define	FIRFILTER_H

#ifndef NULL
#define NULL 0x0
#endif

#include "Filter.h"
#include<iostream>

class FIRFilter : public Filter{
    
    /* ********************************************************************
     * 
     *                          Public Section
     * 
     ******************************************************************** */
public:
    
    /* ********************************************************************
     *                          Constructors
     ******************************************************************** */
    
    /* *********************************************************************
     * Default Constructor: Makes an empty FIRFilter in bypass mode of order
     * 0 with 1.0 set as the only coefficient. 
     * Pre-condition:   None
     * Post-condition:  A new default FIRFilter is returned.
     ********************************************************************* */
    FIRFilter();
    
    /* **********************************************************************
     * Copy Constructor: Returns a deep copy of the parameter FIRFilter.
     * Pre-condition:   The parameter FIRFilter should be initialized, but it
     *                  is not nessesary.
     * Post-condition:  A new deep copy of the parameter FIRFilter is returned.
     *********************************************************************** */
    FIRFilter(const FIRFilter& orig);
    
    /* ********************************************************************
     *                          Destructor
     ******************************************************************** */
    
    /* **********************************************************************
     * Destructor:  Clears all heap-allocated memory this object uses.
     * Pre-condition:   The calling FIRFilter should be initialized.
     * Post-condition:  The calling FIRFilter will release all of it's heap-
     *                  allocated system resources back to the system.
     ********************************************************************** */
    ~FIRFilter();
    
    /* ********************************************************************
     *                          Accessors
     ******************************************************************** */
    
    /* ********************************************************************
     *                          Mutators
     ******************************************************************** */
    
    /* *********************************************************************
     * Push Double: Pushes a double signal into this FIRFilter and returns
     * the output. 
     * Pre-condition:   The parameter double should be within the range of 
     *                  1 and -1 and the calling FIRFilter should be initialized.
     * Post-condition:  The signal will be stored in the appropriate register
     *                  if the FIRFilter's order is greater than 0. A processed
     *                  signal with be outputted.
     ********************************************************************* */
    double pushDouble(double signal);
    
private:

};

#endif	/* FIRFILTER_H */

