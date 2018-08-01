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
//#include "Circuit.h"

#ifndef NULL
#define NULL 0x0
#endif

enum input_type {
    SUM = 0,
    AVERAGE,
    PRODUCT,
    MAX,
    MIN
};

class Circuit;
class Patch;
class InputDevice {
    
/* ****************************************************************
 *                  Public Section
 **************************************************************** */

    
public:
    
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
    
    void setInputType(input_type in);
    
 /* ****************************************************************
 *                  Protected Section
 **************************************************************** */

protected:
    
    LinkedList<double> getInputs() const;
    
    double input() const;
    
private:
    
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

