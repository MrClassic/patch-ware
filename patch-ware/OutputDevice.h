/* ********************************************************************** 
 * File:    OutputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
 *			8/29/19 
 *			Abstracted to interface
 ********************************************************************** */

#ifndef OUTPUTDEVICE_H
#define	OUTPUTDEVICE_H

#include "LinkedList.h"
#include "PatchDevice.h"
#include "Patch.h"
//#include "Circuit.h"

#ifndef NULL
#define NULL 0x0
#endif

class Circuit;
class Patch;
class PatchDevice;
class OutputDevice : public virtual PatchDevice{
    
public:
    
	friend class Patch;

    OutputDevice();
    
    //OutputDevice(Patch * const patch);
    
    //OutputDevice(const OutputDevice &base);
    
	virtual ~OutputDevice() {};
    
    virtual bool addOutput(Patch * const patch) = 0;
    
    virtual bool removeOutput(Patch * const patch) = 0;
    
    virtual int getOutputCount() const = 0;
    
    virtual bool checkOutputs() const = 0;
    
protected:
    
    virtual void output(const double signal) const = 0;
    
    virtual LinkedList<Patch> getOutputPatches() = 0;
    
private:
    
    //static bool outputToPatches(Patch* patch, void* arg);
    
    //static bool checkOutputsPrivate(Patch* patch, void* arg);
    
    //LinkedList<Patch> outputs;

	friend Circuit;

};

#endif	/* OUTPUTDEVICE_H */
