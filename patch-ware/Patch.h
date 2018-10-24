/* ********************************************************************** 
 * File:    Patch.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          1/7/18
 *          File Created
 *          2/28/18
 *          Modified to handle InputDevices and OutputDevices
 *          implemented the signal requesting and pushing
 ********************************************************************** */

#ifndef NULL
#define NULL 0x0
#endif

#ifndef PATCH_H
#define	PATCH_H

//include(s)
#include "InputDevice.h"
#include "OutputDevice.h"

class InputDevice;
class OutputDevice;
/* **********************************************************************
 * Class:   Patch
 * Purpose: To model a patch cable, connecting the output of one effect and
 * the input to another effect.
 ********************************************************************** */
class Patch{
    
    /* ********************************************************************
     * 
     *                          Public Section
     * 
     ******************************************************************** */
    
public:
    
    /* ********************************************************************
     *                          Constructors
     ******************************************************************** */
    
    /* ******************************************************************
     * Default Constructor: Creates a new Patch with no output. 
     * Pre-condition:       None
     * Post-condition:      A new Patch with no output is created.
     ****************************************************************** */
    Patch();
    
    /* ******************************************************************
     * Copy Constructor:    Creates a new Patch that outputs to the same
     * output of the parameterized Patch.
     * Pre-condition:       The parameterized Patch should be initialized.
     * Post-condition:      A copy of the parameterized Patch is created.
     ****************************************************************** */
    Patch(const Patch &p);
    
    /* ********************************************************************
     *                          Destructor
     ******************************************************************** */
    
    /* *******************************************************************
     * Destructor:      Does nothing.
     * Pre-condition:   None.
     * Post-condition:  This Patch is deleted.
     ******************************************************************* */
    ~Patch(){};
    
    /* ********************************************************************
     *                          Accessors
     ******************************************************************** */
    
    InputDevice* getOutput() const;
    
    OutputDevice* getInput() const;

	void getChannels(int &input, int &output);
    
    bool isReady() const;
    
    operator bool() const;
    
    /* ********************************************************************
     *                          Mutators
     ******************************************************************** */
    
    bool requestSignal(double &out);
    
    bool pushSignal(const double signal);
    
    void setInput(OutputDevice *in);
    
    void setOutput(InputDevice* out);
    
private:
    InputDevice *output;
    OutputDevice *input;
    double signal;
    bool ready;
};

#endif	/* PATCH_H */

//EOF