/* **********************************************************************
 * File:    DynamicOutputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *			8/29/19
 *			File created
 ********************************************************************** */

#ifndef DYNAMICOUTPUTDEVICE_H
#define	DYNAMICOUTPUTDEVICE_H

#include "OutputDevice.h"

#ifndef NULL
#define NULL 0x0
#endif

class Circuit;
class Patch;
class PatchDevice;
class DynamicOutputDevice : public virtual OutputDevice {

public:

	friend class Patch;

	DynamicOutputDevice();

	DynamicOutputDevice(Patch * const patch);

	DynamicOutputDevice(const DynamicOutputDevice &base);

	virtual ~DynamicOutputDevice() {};

	virtual bool addOutput(Patch * const patch);

	virtual bool removeOutput(Patch * const patch);

	virtual int getOutputCount() const;

	virtual bool checkOutputs() const;

protected:

	virtual void output(const double signal) const;

	virtual LinkedList<Patch> getOutputPatches();

private:

	static bool outputToPatches(Patch* patch, void* arg);

	static bool checkOutputsPrivate(Patch* patch, void* arg);

	LinkedList<Patch> outputs;

	friend Circuit;

};

#endif	/* OUTPUTDEVICE_H */
