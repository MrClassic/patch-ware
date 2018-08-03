/* ***********************************************************************
 * File:   Filter.h
 * Author: Nate Gallegos
 * 
 * Log
 *      5/2/17
 *      File Created
 ********************************************************************** */

#ifndef NULL
#define NULL 0x0
#endif

#ifndef FILTER_H
#define	FILTER_H

#include<iostream>
#include "Parameter.h"
#include "Effect.h"
#include "Circular.h"

/* **********************************************************************
 * Virtual Class: Filter
 * Purpose: This class is to loosely model the functionality of a general
 * filter.
 ********************************************************************* */
class Filter : public Effect{

    /* ******************************************************************
     * 
     *                          Public Section
     * 
     ****************************************************************** */
public:
    
	Filter();

    /* ******************************************************************
     * Destructor: Declared virtual to ensure that the implemented derived
     * classes handle their own "clean up"
     * Pre-condition:   The derived class should be initialized.
     * Post-condition:  The calling object will release it's allocated resources.
     ****************************************************************** */
    virtual ~Filter();
    
    /* ******************************************************************
     *                              Accessors
     ****************************************************************** */
    
    /* ******************************************************************
     * Get Order: Accessor for the filter's order.
     * Pre-condition:   The filter should be initialized.
     * Post-condition:  The order of the implemented filter will be returned.
     ****************************************************************** */
    int getOrder() const;
    
    /* *******************************************************************
     * Get Coefficient: Accessor for the implemented filter's coefficient at
     * a specific register.
     * Pre-condition:   The calling object should be initialized.
     * Post-condition:  The value of the coefficient at the specified register
     *                  will be returned.
     ******************************************************************* */
    Parameter& getCoefficient(int reg);
    
    /* ******************************************************************
     *                              Mutators
     ****************************************************************** */
    
    /* *******************************************************************
     * Push Double: Pushes a new double into the implemented filter and outputs
     * the result.
     * Pre-condition:   The filter calling object should be initialized.
     * Post-condition:  The parameter double will be pushed into the filter
     *                  calling object, and an output double will be returned.
     ******************************************************************* */
    virtual double pushDouble(double signal)=0;
    
    /* *******************************************************************
     * Process: Processes the double parameter by pushing it into the derived
     * classes implementation of pushDouble().
     * Pre-condition:   The parameter and calling object should be initialized.
     * Post-condition:  A processed signal is returned.
     ******************************************************************** */
    bool process();
    
    /* *******************************************************************
     * Set Order: Mutator for the filter's order. If the order is decreased
     * all data in the registers is lost. If the order is increased the new
     * registers will be set to zero (0) and their coefficients will default
     * to zero (0).
     * Pre-condition:   The Filter calling object should be initialized.
     * Post-condition:  The order of the filter will be changed.
     ******************************************************************** */
    void setOrder(int order);
    
    /* ********************************************************************
     * Set Coefficient: Mutator for the filter's coefficient at a specific 
     * register.
     * Pre-condition:   The Filter calling object should be initialized.
     * Post-condition:  The filter's coefficient at the specific register will
     *                  be changed.
     ******************************************************************** */
    bool setCoefficient(int reg, double coefficient);
    
    /* ********************************************************************
     * Set Coefficient: Mutator for the filter's coefficient at a specific 
     * register.
     * Pre-condition:   The Filter calling object should be initialized.
     * Post-condition:  The filter's coefficient at the specific register will
     *                  be changed.
     ******************************************************************** */
    bool setCoefficient(double coefficient, int reg);
    
    /****************** Used for debugging ********************************/
    
    void printRegisters(std::ostream &os) const;
    
    void printCoefficients(std::ostream &os) const;
    
    /*********************************************************************/
    
protected:
    
    circular_stack<double> registers;
    Parameter* coefficients;
    
};

#endif	/* FILTER_H */

