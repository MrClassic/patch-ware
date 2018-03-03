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
#include "Patch.h"

#ifndef NULL
#define NULL 0x0
#endif
class Patch;
class InputDevice {
    
/* ****************************************************************
 *                  Public Section
 **************************************************************** */

    
public:
    
    InputDevice();
    
    InputDevice(Patch * const patch);
    
    InputDevice(const InputDevice &base);
    
    virtual ~InputDevice(){};
    
    virtual bool addInput(Patch * const patch);
    
    bool removeInput(Patch * const patch);
    
    int getInputCount() const;
    
    bool checkInputs() const;
    
    bool isReady() const;
    
    operator bool() const;
    
    
 /* ****************************************************************
 *                  Protected Section
 **************************************************************** */

protected:
    
    LinkedList<double> getInputs() const;
    
    LinkedList<Patch> getInputPatches() const;
    
    double averageInputs() const;
    
private:
    
    static bool checkInputsPrivate(Patch* data, void* arg);
    
    static bool getInputsPrivate(Patch* patch, void* arg);
    
    LinkedList<Patch> inputs;
    
};

#endif	/* INPUTDEVICE_H */

