/* ********************************************************************
* File:		Circuit.h
* Author:	Nate Gallegos
* 
* The ciruit class is a special case PatchDevice, and sits at the heart
* of the Patch-Ware system. Circuits contain many effects and manage the
* Patch connections between them. I named it a circuit because a visual
* representation of Effects with patches resembles that of a Circuit, 
* where the signal trickles through multiple effects via connections.
* Hopefully one day I get around to documenting this visually in some
* great way that explains this concept well... until then you have to use
* your imagination.
* 
* 
********************************************************************* */

#ifndef CIRCUIT_H
#define CIRCUIT_H


//patchware includes
#include "ProcessorCluster.h"
#include "WaveGenerator.h"
#include "FixedInputDevice.h"
#include "FixedOutputDevice.h"
#include "Effect.h"
#include "Envelope.h"
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

//Circuit class. It does a lot of stuff:
//	Manages many patch devices, but is a patch device it self.
//	Manages the patches that exist between "owned" devices
class Circuit : public FixedInputDevice, public FixedOutputDevice, public Timer{


public:

	//cons and dee
	Circuit();
	Circuit(const unsigned int &channels);
	Circuit(const unsigned int &inputs, const unsigned int &outputs);

	Circuit(const Circuit &);
	~Circuit();

	//PatchDevice lookup
	PatchDevice* getDevice(const std::string&);

	//Timer implementation
	//updates all sub-timers
	void incrementTime(const double time);

	const std::vector<Patch*>& getFirstPatches() { return firstPatches; };

	const std::vector<Patch*>& getLastPatches() { return lastPatches; };

	//add and remove devices
	bool addDevice(const std::string&, PatchDevice* const);
	bool removeDevice(const std::string&);
	
	//patch and unpatch devices
	bool patch(const std::string &output, const std::string &input);
	bool unpatch(const std::string &output, const std::string &input);

	//optimizes circuit path (called implicitly on process)
	void optimize();

	ProcessorCluster* exportAsProcessor();

	//process circuit inputs and send to outputs
	virtual bool process();

	//inherited overloads
	virtual LinkedList<Patch> resizeInputChannels(const unsigned int &channels);
	virtual LinkedList<Patch> resizeOutputChannels(const unsigned int &channels);
	//bool removeOutput(Patch * const);
	//bool removeInput(Patch * const);

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
	//returns string array through "out" pointer,
	//returns array size via return type
	// !!! CREATES THE STRINGS ON THE HEAP !!!
	//You are responsible for cleaning up your mess
	int parse(const std::string &str, std::string *&out) const;

	//increments circuit level recursively
	void incrementLevel();

	//LinkedList Apply Functions
	static bool processInOrder(PatchDevice* pd, void* args);
	//static bool getInputs(Patch* patch, void* args);
	//static bool sendOutputs(Patch* patch, void* args);

	//my variables, no touchie!!!
	bool changed = false;
	std::map<std::string, PatchDevice*> devices;
	LinkedList<Patch> patch_master;
	LinkedList<PatchDevice> order;
	//LinkedList<Processor> processOrder;
	LinkedList<Timer> timers;
	std::vector<Patch*> firstPatches;
	std::vector<Patch*> lastPatches;
	int level;

};

#endif

//EOF