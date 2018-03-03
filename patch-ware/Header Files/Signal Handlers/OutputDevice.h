/* ********************************************************************** 
 * File:    OutputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          2/27/18
 *          File Created
 * 
 ********************************************************************** */

#ifndef OUTPUTDEVICE_H
#define	OUTPUTDEVICE_H

#include "LinkedList.h"
#include "Patch.h"

#ifndef NULL
#define NULL 0x0
#endif
class Patch;
class OutputDevice{
    
public:
    
    OutputDevice();
    
    OutputDevice(Patch * const patch);
    
    OutputDevice(const OutputDevice &base);
    
    virtual ~OutputDevice(){};
    
    virtual bool addOutput(Patch * const patch);
    
    virtual bool removeOutput(Patch * const patch);
    
    int getOutputCount() const;
    
    bool checkOutputs() const;
    
protected:
    
    void output(const double signal) const;
    
    LinkedList<Patch> getOutputPatches() const;
    
private:
    
    static bool outputToPatches(Patch* patch, void* arg);
    
    static bool checkOutputsPrivate(Patch* patch, void* arg);
    
    LinkedList<Patch> outputs;
};

#endif	/* OUTPUTDEVICE_H */
