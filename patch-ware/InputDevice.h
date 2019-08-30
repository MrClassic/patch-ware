/* ********************************************************************** 
 * File:    InputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
 *			8/29/19 
 *			Abstracted to interface
 * 
 ********************************************************************** */

#ifndef INPUTDEVICE_H
#define	INPUTDEVICE_H

#include "LinkedList.h"
#include "PatchDevice.h"
#include "Patch.h"
#include "InputTypes.h"
//#include "Circuit.h"

#ifndef NULL
#define NULL 0x0
#endif


class Circuit;
class Patch;
class PatchDevice;
class InputDevice : public virtual PatchDevice{
    
/* ****************************************************************
 *                  Public Section
 **************************************************************** */

    
public:
    
	friend class Patch;

    InputDevice();
    
	virtual ~InputDevice() {};
    
    virtual bool addInput(Patch * const patch) = 0;
    
    virtual bool removeInput(Patch * const patch) = 0;
    
    virtual int getInputCount() const = 0;
    
    virtual bool checkInputs() const;
    
    virtual bool isReady() const = 0;
    
    virtual operator bool() const;

	virtual LinkedList<Patch> getInputPatches() = 0;
    
    virtual void setInputType(input_type in);
    
 /* ****************************************************************
 *                  Protected Section
 **************************************************************** */

protected:
    
    virtual LinkedList<double> getInputs() const = 0;
    
    virtual double input() const;
    
    virtual double averageInputs() const;
    
    virtual double maxInput() const;
    
    virtual double minInput() const;
    
    virtual double sumInputs() const;
    
    virtual double multiplyInputs() const;
    
    //LinkedList<Patch> inputs;
    
    input_type inputType;
    
	friend Circuit;
};


#endif	/* INPUTDEVICE_H */

