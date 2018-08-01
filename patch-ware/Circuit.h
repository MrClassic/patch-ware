#ifndef CIRCUIT_H
#define CIRCUIT_H


//patchware includes
#include "WaveGenerator.h"
#include "BiquadFilter.h"
#include "Compressor.h"
#include "Delay.h"
#include "Gain.h"
#include "Gate.h"
#include "InvertDistortion.h"
#include "SignalSpy.h"

//std includes
#include <string>
#include <map>
#include <vector>

class Circuit : public Effect {


public:

	Circuit();
	Circuit(const Circuit &);
	~Circuit();

	OutputDevice* getDevice(const std::string&);

	void incrementTime(const double time);

	bool addDevice(const std::string&, OutputDevice* const);
	bool removeDevice(const std::string&);
	
	bool patch(const std::string &output, const std::string &input);
	bool unpatch(const std::string &output, const std::string &input);

	void optimize();
	bool process();

	//inherited overloads
	bool removeOutput(Patch * const);
	bool removeInput(Patch * const);

private:

	InputDevice * toInputDevice(OutputDevice * const) const;
	WaveGenerator* toWaveGenerator(OutputDevice * const) const;
	Effect* toEffect(OutputDevice * const) const;

	OutputDevice* toOutputDevice(InputDevice * const) const;
	Effect* toEffect(InputDevice* const) const;
	Parameter* toParameter(InputDevice* const) const;

	void removePatch(Patch * const);
	bool getIODevices(const std::string &, const std::string& , OutputDevice* &, InputDevice* &, int &, int &);

	//private method to pass to LinkedList.apply()
	//bool processOrder(Patch*, void*);

	//variables
	bool changed = false;
	std::map<std::string, OutputDevice*> devices;
	LinkedList<Patch> patch_master;
	LinkedList<Patch> order;
	std::vector<Patch*> firstPatches;
	std::vector<Patch*> lastPatches;
};


enum EFFECTS {
	BIQUAD_FILTER = 0,
	CIRCUIT,
	COMPRESSOR,
	DELAY,
	DISTORTION,
	FIR_FILTER,
	GAIN,
	GATE,
	IIR_FILTER,
	INVERT_DISTORTION,
	SIGNAL_SPY,

	//... insert new Effects as they are made

	NUM_OF_EFFECTS
};

EFFECTS getEffectType(OutputDevice* device);

#endif