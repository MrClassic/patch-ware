/* **********************************************************************
 * File:    DynamicInputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          8/29/19
 *          File Created
 *
 ********************************************************************** */

#ifndef DYNAMICINPUTDEVICE_H
#define DYNAMICINPUTDEVICE_H

#include "InputDevice.h"

#ifndef NULL
#define NULL 0x0
#endif


class Circuit;
class Patch;
class PatchDevice;
class DynamicInputDevice : public virtual InputDevice {

	/* ****************************************************************
	 *                  Public Section
	 **************************************************************** */


public:

	friend class Patch;

	DynamicInputDevice();

	DynamicInputDevice(Patch * const patch);

	DynamicInputDevice(const DynamicInputDevice &base);

	virtual ~DynamicInputDevice() {};

	virtual bool addInput(Patch * const patch);

	virtual bool removeInput(Patch * const patch);

	int getInputCount() const;

	bool isReady() const;

	LinkedList<Patch> getInputPatches();

	/* ****************************************************************
	*                  Protected Section
	**************************************************************** */

protected:

	LinkedList<double> getInputs() const;

	//double input() const;

	static bool checkInputsPrivate(Patch* data, void* arg);

	static bool getInputsPrivate(Patch* patch, void* arg);

	LinkedList<Patch> inputs;

	friend Circuit;

};
#endif