
//patchware includes
#include "IncludeEffects.h"
#include "WaveGenerator.h"

#ifndef CIRCUIT_H
#define CIRCUIT_H

//std includes
#include <string>
#include <map>



class Circuit : public Effect{


public:

	Circuit();
	Circuit(const Circuit &);

	OutputDevice* getDevice(const std::string);

	bool addDevice(const std::string, OutputDevice* const);
	bool removeDevice(const std::string, OutputDevice* const);
	bool patch(const std::string output, const std::string input);

	void optimize();
	bool process();

private:

	InputDevice* toInputDevice(OutputDevice * const);
	WaveGenerator* toWaveGenerator(OutputDevice * const);
	Effect* toEffect(OutputDevice * const);

	//private method to pass to LinkedList.apply()
	bool processOrder(Patch*, void*);

	//variables
	std::map<std::string, OutputDevice*> devices;
	LinkedList<Patch> patch_master;
	LinkedList<Patch> order;
	LinkedList<Patch> firstPatches;
	LinkedList<Patch> lastPatches;
};

#endif