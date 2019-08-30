/* **********************************************************************
 * File:    FixedInputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          8/29/19
 *          File Created
 *
 ********************************************************************** */


#ifndef FIXEDINPUTDEVICE_H
#define FIXEDINPUTDEVICE_H

#include "InputDevice.h"
#include <vector>


class FixedInputDevice : public virtual InputDevice {

public:

	friend class Patch;

	FixedInputDevice();

	FixedInputDevice(const unsigned int &channels);

	FixedInputDevice(const FixedInputDevice &base);

	virtual ~FixedInputDevice() {};

	virtual bool addInput(Patch * const patch);

	virtual Patch* addInput(Patch * const patch, const unsigned int &channel);

	virtual bool removeInput(Patch * const patch);

	virtual int getInputCount() const;

	virtual LinkedList<Patch> resizeInputChannels(const unsigned int &channels);

	virtual bool isReady() const;

	virtual LinkedList<Patch> getInputPatches();

	/* ****************************************************************
	*                  Protected Section
	**************************************************************** */

protected:

	virtual LinkedList<double> getInputs() const;

	std::vector<Patch*> inputs;

	friend Circuit;

};

#endif