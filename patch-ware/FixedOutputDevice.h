/* **********************************************************************
 * File:    FixedOutputDevice.h
 * Author:  Nate Gallegos
 *
 * Log:
 *          8/29/19
 *          File Created
 *
 ********************************************************************** */

#ifndef FIXEDOUTPUTDEVICE_H
#define	FIXEDOUTPUTDEVICE_H

#include "OutputDevice.h"
#include <vector>

#ifndef NULL
#define NULL 0x0
#endif

class Circuit;
class Patch;
class PatchDevice;
class FixedOutputDevice : public virtual OutputDevice {

public:

	friend class Patch;

	FixedOutputDevice();

	FixedOutputDevice(const unsigned int &channels);

	FixedOutputDevice(const FixedOutputDevice &base);

	virtual ~FixedOutputDevice() {};

	virtual bool addOutput(Patch * const patch);

	virtual Patch* addOutput(Patch * const patch, const unsigned int &channel);

	virtual bool removeOutput(Patch * const patch);

	virtual LinkedList<Patch> resizeOutputChannels(const unsigned int &channels);

	int getOutputCount() const;

	bool checkOutputs() const;

protected:

	void output(const double signal) const;

	void output(const std::vector<double> signals) const;

	LinkedList<Patch> getOutputPatches();

	std::vector<Patch*> outputs;

	friend Circuit;

};

#endif	/* OUTPUTDEVICE_H */
