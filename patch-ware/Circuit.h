#ifndef CIRCUIT_H
#define CIRCUIT_H


//patchware includes
#include "WaveGenerator.h"
#include "Effect.h"
#include "pwmath.h"

//std includes
#include <string>
#include <map>
#include <vector>

class InputDevice;
class OutputDevice;
class Timer;
class Effect;
class WaveGenerator;
class Patch;
class PatchDevice;
class Circuit : public InputDevice, public OutputDevice, public Timer {


public:

	//cons and dee
	Circuit();
	Circuit(const Circuit &);
	~Circuit();

	//PatchDevice lookup
	PatchDevice* getDevice(const std::string&);

	//Timer implementation
	//updates all sub-timers
	void incrementTime(const double time);

	//add and remove devices
	bool addDevice(const std::string&, OutputDevice* const);
	bool removeDevice(const std::string&);
	
	//patch and unpatch devices
	bool patch(const std::string &output, const std::string &input);
	bool unpatch(const std::string &output, const std::string &input);

	//optimizes circuit path (called implicitly on process)
	void optimize();

	//process circuit inputs and send to outputs
	virtual bool process();

	//inherited overloads
	bool removeOutput(Patch * const);
	bool removeInput(Patch * const);

private:

	//pointer casting functions
	InputDevice * toInputDevice(PatchDevice * const) const;
	WaveGenerator* toWaveGenerator(PatchDevice * const) const;
	OutputDevice * toOutputDevice(PatchDevice * const) const;
	Effect* toEffect(PatchDevice * const) const;
	Parameterizable* toParameterizable(PatchDevice* const) const;
	Timer* toTimer(PatchDevice* const) const;
	Circuit* toCircuit(PatchDevice * const) const;
	Parameter* toParameter(PatchDevice * const) const;

	//lookup methods
	InputDevice* lookupInputDevice(std::string *strings, int size, int& inputChannel);
	OutputDevice* lookupOutputDevice(std::string *strings, int size, int& outputChannel);
	
	//patch removal
	void removePatch(Patch * const);

	//string parser. parses based on ':'
	int parse(const std::string &str, std::string *&out) const;

	//increments circuit level recursively
	void incrementLevel();

	//LinkedList Apply Functions
	static bool processInOrder(PatchDevice* pd, void* args);
	static bool getInputs(Patch* patch, void* args);
	static bool sendOutputs(Patch* patch, void* args);

	//my variables, no touchie!!!
	bool changed = false;
	std::map<std::string, PatchDevice*> devices;
	LinkedList<Patch> patch_master;
	LinkedList<PatchDevice> order;
	LinkedList<Timer> timers;
	std::vector<Patch*> firstPatches;
	std::vector<Patch*> lastPatches;
	int level;

};

#endif

//EOF