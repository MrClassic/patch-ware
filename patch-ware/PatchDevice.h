/*
	File: PatchDevice.h
	Purpose: To provide a single pointer type to reference both Input and Output Devices
		while accessing the process() method

	Log:
		8/12/18 - N8
			File Created

*/

#ifndef PATCHDEVICE_H
#define PATCHDEVICE_H

//#include "Patch.h"

class PatchDevice {
public:


	virtual ~PatchDevice() {/* Do Nothing */};

	virtual bool process() = 0; //virtual process method

protected:

	//protected constructors to force no construction of PatchDevices explicitly

	PatchDevice() {};

	PatchDevice(const PatchDevice& rhs) {};

};

#endif // !PATCHDEVICE_H
