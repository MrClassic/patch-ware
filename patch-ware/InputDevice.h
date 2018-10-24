/* ********************************************************************** 
 * File:    InputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
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
    
    InputDevice(Patch * const patch);
    
    InputDevice(const InputDevice &base);
    
	virtual ~InputDevice() {};
    
    virtual bool addInput(Patch * const patch);
    
    virtual bool removeInput(Patch * const patch);
    
    int getInputCount() const;
    
    bool checkInputs() const;
    
    bool isReady() const;
    
    operator bool() const;

	LinkedList<Patch> getInputPatches();
    
    virtual void setInputType(input_type in);
    
 /* ****************************************************************
 *                  Protected Section
 **************************************************************** */

protected:
    
    LinkedList<double> getInputs() const;
    
    double input() const;
    

    
    static bool checkInputsPrivate(Patch* data, void* arg);
    
    static bool getInputsPrivate(Patch* patch, void* arg);
    
    double averageInputs() const;
    
    double maxInput() const;
    
    double minInput() const;
    
    double sumInputs() const;
    
    double multiplyInputs() const;
    
    LinkedList<Patch> inputs;
    
    input_type inputType;
    
	friend Circuit;
};


#endif	/* INPUTDEVICE_H */

