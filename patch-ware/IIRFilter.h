/* ************************************************************************
 * File:   IIRFilter.cpp
 * Author: Nate Gallegos
 * 
 * Log
 *      5/4/17
 *      File Created
 ************************************************************************ */

#ifndef IIRFILTER_H
#define	IIRFILTER_H

#ifndef NULL
#define NULL 0x0
#endif

#include "Filter.h"
#include<iostream>

class IIRFilter : public Filter{
    
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
     * Default Constructor: Makes an empty IIRFilter in bypass mode of order
     * 0 with 1.0 set as the only coefficient. 
     * Pre-condition:   None
     * Post-condition:  A new default IIRFilter is returned.
     ********************************************************************* */
    IIRFilter();
    
    /* **********************************************************************
     * Copy Constructor: Returns a deep copy of the parameter IIRFilter.
     * Pre-condition:   The parameter IIRFilter should be initialized, but it
     *                  is not nessesary.
     * Post-condition:  A new deep copy of the parameter IIRFilter is returned.
     *********************************************************************** */
    IIRFilter(const IIRFilter& orig);
    
    /* ********************************************************************
     *                          Destructor
     ******************************************************************** */
    
    /* **********************************************************************
     * Destructor:  Clears all heap-allocated memory this object uses.
     * Pre-condition:   The calling IIRFilter should be initialized.
     * Post-condition:  The calling IIRFilter will release all of it's heap-
     *                  allocated system resources back to the system.
     ********************************************************************** */
    ~IIRFilter();
    
    /* ********************************************************************
     *                          Accessors
     ******************************************************************** */
    
    /* ********************************************************************
     *                          Mutators
     ******************************************************************** */
    
    /* *********************************************************************
     * Push Double: Pushes a double signal into this IIRFilter and returns
     * the output. 
     * Pre-condition:   The parameter double should be within the range of 
     *                  1 and -1 and the calling IIRFilter should be initialized.
     * Post-condition:  The signal will be stored in the appropriate register
     *                  if the IIRFilter's order is greater than 0. A processed
     *                  signal with be outputted.
     ********************************************************************* */
    double pushDouble(double signal);
    
private:

};

#endif	/* FIRFILTER_H */

