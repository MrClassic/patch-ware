/* ************************************************************************
 * File:   Controller.h
 * Author: Nate Gallegos
 * 
 * Log 
 *      1/9/18
 *      File Created
 *********************************************************************** */

#include "Effect.h"

#ifndef NULL
#define NULL 0x0
#endif

#ifndef CONTROLLER_H
#define	CONTROLLER_H

/* ************************************************************************
 * Class:       Controller
 * Purpose:     To model a parameter controller. The controller is designed
 *              to wrap around a single pointer to a double that is meant to
 *              be pointed to a parameter of an Effect.
 *              The idea is to have a controller act as an intermediate layer
 *              that can use a signal (audio signal or a wave generator signal)
 *              to controller an Effect's parameter.
 ************************************************************************* */

class Controller : public Effect{
    
public:
    
    /* ********************************************************************
     * Default Constructor: Simply initializes the Controller and sets its
     * parameter pointer to NULL.
     * Pre-conditions:  None
     * Post-conditions: A new Controller that does not point to any parameter
     * will be created and returned.
     ******************************************************************** */
    Controller(){
        param = NULL;
    }
    
    /* ********************************************************************
     * Constructor:     Simply initializes the Controller and sets its
     * parameter pointer to the double given.
     * Pre-conditions:  None
     * Post-conditions: A new Controller with an initialized double parameter
     * will be created and returned.
     ******************************************************************** */
    Controller(double &p){
        param = p;
    }
    
    /* ********************************************************************
     * Get Value:       Accessor for the current value of the paramter this
     * Controller is controlling. If the Controller does not point to any 
     * parameter, zero (0) is returned.
     * Pre-conditions:  None
     * Post-conditions: The value of the parameter being controlled will
     * be returned. Zero is returned if no parameter is being controlled.
     ******************************************************************** */
    double getValue(){
        if(param == NULL)
            return 0.0;
        return *param;
    }
    
    /* ********************************************************************
     * Process:         Pushes the input signal into the value of the 
     * controlled parameter.
     * Pre-conditions:  None
     * Post-conditions: The controlled parameter will be assigned the value
     * in the signal variable.
     ******************************************************************** */
    void process(double signal){
        if(param != NULL)
            *param = signal;
    }
    
    /* ********************************************************************
     * Set Param:       Sets this Controller's controlled parameter to the
     * double given.
     * Pre-conditions:  The address of parameter "p" should be a valid memory
     * address of a double.
     * Post-conditions: This Controller's controlled parameter will be set to
     * the double value at the address given.
     ******************************************************************** */
    void setParam(double &p){
        param = p;
    }
    
    /* ********************************************************************
     * Set Value:       Sets this Controller's controlled parameter's value
     * to the given value.
     * Pre-conditions:  None
     * Post-conditions: This Controller's controlled parameter's value will
     * be updated to the value given.
     ******************************************************************** */
    void setValue(double value){
        if(param != NULL)
            *param = value;
    }
    
private:
    
    //parameter pointer
    double *param;
};

#endif	/* CONTROLLER_H */

//EOF