/*
File:	Circuit.h
Author:	Nate Gallegos

Log:
		7/21/18
		File created
		7/27/18
		started implementing functionality
		implemented patch()
			-- TODO: handle patching into the circuit's inputs and outputs
		7/29/18
		continued basic implementation
		implemented optimize()
		8/1/18
		revised some documentation
		implemented unpatch()
		abstracted some functionality into getIODevcies()
		add incrementTime() to support on board wave generators
		8/17/18
		implemented Timer/Parameterizable update
*/
#include "Circuit.h"

Circuit::Circuit() {
	level = 0;
}

Circuit::Circuit(const Circuit &other) {
	level = 0;
	//TODO: this!
	//deep copy all patches and devices contained by the "other" circuit
	//including any sub-circuits
}

//clean up yo mess!!!
Circuit::~Circuit() {

	//cut external patches from referencing this Circuit
	LinkedList<Patch> inputs = getInputPatches();
	while (!inputs.isEmpty()) {
		inputs.pop_front()->setOutput(NULL);
	}
	LinkedList<Patch> outputs = getOutputPatches();
	while (!outputs.isEmpty()) {
		outputs.pop_front()->setInput(NULL);
	}

	//clean up my internal components
	
	for (std::pair<std::string, PatchDevice*> p : devices) {
		delete p.second;
		p.second = NULL;
	}

	firstPatches.clear();
	lastPatches.clear();
	patch_master.clear(true);
}

PatchDevice* Circuit::getDevice(const std::string &tag) {
	auto search = devices.find(tag);
	if (search == devices.end()) {
		return NULL;
	}
	return search->second;
}

bool Circuit::removeDevice(const std::string &tag) {

	/*
	//TODO: implement this!!!!

	OutputDevice* od = getDevice(tag);
	if (od == NULL) {
		return false;
	}

	LinkedList<Patch> patches;//patches to disconnect

	*/
	return false;
}

//increment time for all on board wave generators and call
//recursively to any on board sub-Circuits
void Circuit::incrementTime(const double time) {
	for (auto p : devices) {
		if (toTimer(p.second) != NULL) {
			Timer* timer = toTimer(p.second);
			timer->incrementTime(time);
		}
	}
}

//attempts to add the device to this circuit with the tag provided.
//If the tag is already in use, false is returned, else true is returned.
bool Circuit::addDevice(const std::string &tag, OutputDevice* const device) {
	if (tag.find(':') != std::string::npos || tag == "in" || tag == "out") {
		return false;
	}
	if (devices[tag] == NULL) {
		devices[tag] = device;
		if (toCircuit(device) != NULL) {
			Circuit *c = toCircuit(device);
			c->incrementLevel();
		}
		return true;
	}
	return false;
}

//attempts to patch the output of the output device to the input device.
//if teither device is not present within this circuit or the input and 
//output parameters don't match input or output parameters respectively
//false is returned.
//for patching into parameters, use OutputDevice:Parameter format, e.g.
//		string input = "myWaveGenerator:Frequency"
//will represent the frequency parameter for the wave generator called
//"myWaveGenerator"
//For patching into the "outside world" beyond this circuit, use:
//		in:n OR
//		out:n
//where "n" == the input or output channel respectively.
//
//NOTE: The output string can pipe to the input of the circuit, but not the
//output of the circuit (I know this is confusing, bare with me).
//		string output = "in:0" 
//will attach one end of the Patch to Input Channel 0, and
//		string input = "out:1"
//will attach one end of the Patch to Output Channel 1.
//for an effective bypass of the circuit, consider the following:
//		string output = "in:0"
//		string input = "out:0"
//which will take the signal from input channel 0 and patch it directly
//to output channel 0 since the input into the Patch being created is 
//actually the "output" string, and vice versa for the "input" string.
//
//						        [Signal Flow]
// [OutputDevice] >>>>>>>>>>>>>>>>  Patch  >>>>>>>>>>>>>>>>> [InputDevice]
// (string output)			(patch being created)			 (string input)
//
// For a better diagram, see the Cicuit documentation
// (not made yet... 404 document not found)
bool Circuit::patch(const std::string &output, const std::string &input) {
	
	//The input and output channels IF the patch being added is in
	//"firstPatches" or "lastPatches"
	int iChannel = -1;
	int oChannel = -1;

	std::string* iStrings = NULL;
	int iSize = parse(input, iStrings);  // <--- creates string[] on heap !!!!!

	std::string* oStrings = NULL;
	int oSize = parse(output, oStrings); // <--- creates string[] on heap !!!!!

	
	//valid results from parsing strings
	if (pw_abs(iSize - oSize) > 1) {
		//devices too different in scope
		//i.e. input = "myCircuit:myWaveGen:frequency"
		//     output = "myEffect"
		//Patch would cross through circuits
		
		QUIT:	// <---------------------------------------------- Label: Quit
		delete[] oStrings;
		delete[] iStrings;
		return false;
	}

	//the OutputDevice and InputDevice sides of the patch being added
	OutputDevice* o = lookupOutputDevice(oStrings, oSize, oChannel);
	InputDevice* i = lookupInputDevice(iStrings, iSize, iChannel);

	//validate that we're not patching illegally between circuits
	Circuit *oCircuit = NULL;
	Circuit* iCircuit = NULL;
	int oCircDepth = 0;
	int iCircDepth = 0;

	//temp Circuit pointer "the explorer"
	Circuit *temp = this;

	//set oCircuit to the deepest circuit described by oStrings
	while (temp != NULL) {
		oCircuit = temp;
		temp = toCircuit(oCircuit->getDevice(oStrings[oCircDepth++]));
	}

	//reset temp variable
	temp = this;

	//set iCircuit to the deepest circuit described by iStrings
	while (temp != NULL) {
		iCircuit = temp;
		temp = toCircuit(iCircuit->getDevice(iStrings[iCircDepth++]));
	}

	//Circuit pointer to point to the circuit that will do the patching internally
	Circuit* parentCircuit = NULL;

	if (iCircuit->level > oCircuit->level) {

		//look for child circuit
		bool found = false;
		for (auto it = oCircuit->devices.begin(); it != oCircuit->devices.end(); ++it) {
			if (it->second == iCircuit) {
				found = true;
				break;
			}
		}

		//if not found, cannot patch devices
		if (!found) {
			goto QUIT;
		}

		//OutputDevice's circuit is parent
		parentCircuit = oCircuit;
	}
	else if (oCircuit->level > oCircuit->level) {

		//look for child circuit
		bool found = false;
		for (auto it = iCircuit->devices.begin(); it != iCircuit->devices.end(); ++it) {
			if (it->second == oCircuit) {
				found = true;
				break;
			}
		}

		//if not found, cannot patch devices
		if (!found) {
			goto QUIT;
		}

		//OutputDevice's circuit is parent
		parentCircuit = iCircuit;
	}
	else {

		//oCircuit and iCircuit on same level.

		//we need to find their parents, then make sure
		//they have the same parent, and set it as the parent pointer

		Circuit *explorer = this;
		int depth = -1;

		int iDepth = iSize - 1;
		if (iChannel >= 0) {
			iDepth--;
		}


		int oDepth = oSize - 1;
		if (oChannel >= 0) {
			oDepth--;
		}

		while (explorer != NULL) {
			depth++;
			parentCircuit = explorer;
			Circuit* iCirc = toCircuit(parentCircuit->getDevice(iStrings[depth]));
			if (iCirc == NULL) {
				break;
			}
			explorer = toCircuit(parentCircuit->getDevice(oStrings[depth]));
			if (explorer == NULL) {
				break;
			}
			

			if (iCirc != explorer) {
				if (oDepth - 1 == depth && iDepth - 1 == depth)
					//if two circuits at same level AND they both
					//represent the deepest 
					break;

				else
					//two different circuits, but different device 
					return false;
			}
		}

	}
	
	//don't need these any more...
	delete[] oStrings;
	delete[] iStrings;

	if (o == NULL || i == NULL) {
		
		return false;
	}

	Patch * patch = new Patch;

	

	//Set the input of the new patch
	if (oChannel == -1) {

		//not patching to "firstPatches"
		o->addOutput(patch);

	}
	else {
		
		//make sure there is a slot in the vector for oChannel
		while (parentCircuit->firstPatches.size() < oChannel + 1) {
			parentCircuit->firstPatches.push_back(NULL);
		}

		patch->setInput(NULL);//input into patch NULL. Input handled explicitly by the Circuit class
		parentCircuit->firstPatches[oChannel] = patch;

	}

	//Set the output of the new patch
	if (iChannel == -1) {

		//not patching to "firstPatches"
		i->addInput(patch);

	}
	else {
		//patching into "output" channel (lastPatches)

		//make sure there is a slot in the vector for oChannel
		while (parentCircuit->lastPatches.size() < iChannel + 1) {
			parentCircuit->lastPatches.push_back(NULL);
		}

		patch->setOutput(NULL);//input into patch NULL. Input handled explicitly by the Circuit class
		parentCircuit->lastPatches[iChannel] = patch;

	}

	parentCircuit->patch_master.push_back(patch);
	parentCircuit->changed = true;
	return true;

}

//same parameter conventions as patch()
//see patch() function description
bool Circuit::unpatch(const std::string &output, const std::string &input) {
	

	//The input and output channels IF the patch being added is in
	//"firstPatches" or "lastPatches"
	int iChannel = -1;
	int oChannel = -1;

	std::string* iStrings = NULL;
	int iSize = parse(input, iStrings);  // <--- creates string[] on heap !!!!!

	std::string* oStrings = NULL;
	int oSize = parse(output, oStrings); // <--- creates string[] on heap !!!!!


										 //valid results from parsing strings
	if (pw_abs(iSize - oSize) > 1) {
		//devices too different in scope
		//i.e. input = "myCircuit:myWaveGen:frequency"
		//     output = "myEffect"
		//Patch would cross through circuits

	QUIT:	// <---------------------------------------------- Label: Quit
		delete[] oStrings;
		delete[] iStrings;
		return false;
	}

	//the OutputDevice and InputDevice sides of the patch being added
	OutputDevice* o = lookupOutputDevice(oStrings, oSize, oChannel);
	InputDevice* i = lookupInputDevice(iStrings, iSize, iChannel);

	//validate that we're not patching illegally between circuits
	Circuit *oCircuit = NULL;
	Circuit* iCircuit = NULL;
	int oCircDepth = 0;
	int iCircDepth = 0;

	//temp Circuit pointer "the explorer"
	Circuit *temp = this;

	//set oCircuit to the deepest circuit described by oStrings
	while (temp != NULL) {
		oCircuit = temp;
		temp = toCircuit(oCircuit->getDevice(oStrings[oCircDepth++]));
	}

	//reset temp variable
	temp = this;

	//set iCircuit to the deepest circuit described by iStrings
	while (temp != NULL) {
		iCircuit = temp;
		temp = toCircuit(iCircuit->getDevice(iStrings[oCircDepth++]));
	}

	//Circuit pointer to point to the circuit that will do the patching internally
	Circuit* parentCircuit = NULL;

	if (iCircuit->level > oCircuit->level) {

		//look for child circuit
		bool found = false;
		for (auto it = oCircuit->devices.begin(); it != oCircuit->devices.end(); ++it) {
			if (it->second == iCircuit) {
				found = true;
				break;
			}
		}

		//if not found, cannot patch devices
		if (!found) {
			goto QUIT;
		}

		//OutputDevice's circuit is parent
		parentCircuit = oCircuit;
	}
	else if (oCircuit->level > oCircuit->level) {

		//look for child circuit
		bool found = false;
		for (auto it = iCircuit->devices.begin(); it != iCircuit->devices.end(); ++it) {
			if (it->second == oCircuit) {
				found = true;
				break;
			}
		}

		//if not found, cannot patch devices
		if (!found) {
			goto QUIT;
		}

		//OutputDevice's circuit is parent
		parentCircuit = iCircuit;
	}
	else {

		//oCircuit and iCircuit on same level.

		//we need to find their parents, then make sure
		//they have the same parent, and set it as the parent pointer

		Circuit *explorer = this;
		int depth = -1;

		int iDepth = iSize - 1;
		if (iChannel >= 0) {
			iDepth--;
		}


		int oDepth = oSize - 1;
		if (oChannel >= 0) {
			oDepth--;
		}

		while (explorer != NULL) {
			depth++;
			parentCircuit = explorer;
			Circuit* iCirc = toCircuit(parentCircuit->getDevice(iStrings[depth]));
			if (iCirc == NULL) {
				break;
			}
			explorer = toCircuit(parentCircuit->getDevice(oStrings[depth]));
			if (explorer == NULL) {
				break;
			}


			if (iCirc != explorer) {
				if (oDepth - 1 == depth && iDepth - 1 == depth)
					//if two circuits at same level AND they both
					//represent the deepest 
					break;

				else
					//two different circuits, but different device 
					return false;
			}
		}

	}

	//don't need these any more...
	delete[] oStrings;
	delete[] iStrings;

	if (o == NULL || i == NULL) {

		return false;
	}

	LinkedList<Patch> candidates;

	//check output device
	if (oChannel < 0) {
		//if not an edge patch, add possible patches to candidate list
		candidates = candidates + o->getOutputPatches();
	}
	else {

		//if edge patch (firstPatches) 
		if (firstPatches[oChannel] != NULL) {

			//test if the patch is described correctly
			if (firstPatches[oChannel]->getOutput() == i) {

				//remove the patch
				removePatch(firstPatches[oChannel]);

				//set slot to NULL
				firstPatches[oChannel] = NULL;

				if (iChannel > -1) {
					//lastPatches needs to be updated too
					lastPatches[iChannel] = NULL;
				}

				//the circuit has changed
				changed = true;

				//success (...?)
				return true;
			}
			else {
				//patch is not described correctly
				return false;
			}
		}
		else {
			//there is no patch in that channel
			return false;
		}
	}

	//check input device
	if (iChannel < 0) {
		//not an edge patch, add possible patches to candidate list
		candidates = candidates + i->getInputPatches();
	}
	else {

		//if edge patch (lastPatches)
		if (lastPatches[iChannel] != NULL) {

			//test if the patch is described correctly
			if (lastPatches[iChannel]->getInput() == o) {

				//remove the patch
				removePatch(lastPatches[iChannel]);

				//set slot to NULL
				lastPatches[iChannel] = NULL;

				if (oChannel > -1) {
					//firstPatches needs to be updated too
					//(Will never get here)
					firstPatches[oChannel] = NULL;
				}

				//the circuit has changed
				changed = true;

				//success (...?)
				return true;
			}
			else {
				//patch not described correctly
				return false;
			}
		}
		else {
			//no patch in output channel
			return false;
		}
	}

	//if we got here, we have candidate patches to look through
	while (!candidates.isEmpty()) {
		Patch* pop = candidates.pop_front();

		//REMEMBER: o is the output device whose output is the input into this patch
		//and i is the input device whose input is the output of this patch
		//confusing I know... FML!
		if (pop != NULL && pop->getInput() == o && pop->getOutput() == i) {
			//found the patch we're looking for!
			//remove it, flag the circuit as changed and return true!
			removePatch(pop);
			changed = true;
			return true;// <-- Mission accomplished!
		}
	}

	//no patch matched what we were looking for...?
	//hmmm... somethinf might have gone wrong...
	return false;

}

void Circuit::optimize() {

	if (!changed) {
		//no changes, no need to optimize the circuit path.
		return;
	}
	//queue for breadth-first reverse traversal of the circuit
	//starts at the end and makes its way to the beginning
	LinkedList<Patch> queue;// = lastPatches;
	for (Patch* p : lastPatches) {
		queue.push_back(p);
	}

	//map to track which PatcheDevice have already been visited
	std::map<PatchDevice*, bool> visited;

	//loop until queue is empty (i.e. the whole circuit has been visited)
	while (!queue.isEmpty()) {

		//input devices to gather next set of patches from
		LinkedList<InputDevice> iDevices;

		//next pop from the queue
		Patch* pop = queue.pop_front();
		if (pop == NULL) {
			continue;
		}

		//patch device associated with the popped Patch
		PatchDevice* device = pop->getInput();

		//if there's no device, continue
		if (device == NULL) {
			continue;
		}

		//Add any parameters' patches to the queue
		Parameterizable* p = toParameterizable(device);
		if (p != NULL) {
			LinkedList<Parameter> params = p->getParameters();
			while (!params.isEmpty()) {
				Parameter* pop = params.pop_front();
				if (pop == NULL || !pop->isPatched()) {
					continue;
				}
				iDevices.push_back(pop);
			}
		}

		if (toInputDevice(device) != NULL) {

			//hanlde Input Device
			InputDevice* in = toInputDevice(device);
			if (in != NULL && in->getInputCount() > 0) {
				iDevices.push_back(in);
			}
		}

		if (visited[device]) {

			//remove the visited patch from the "order" list
			//so the patch can be put at the front
			order.remove(device);
			
		}
		else {
			//set as vistied
			visited[device] = true;
		}
		//add patch to the front of the order list.
		//front of the list == higher priority when 
		//running a signal through the circuit.
		order.push_front(device);

		//gathered all Input Devices into iDevices!
		
		//for each found InutDevice, get its input patches and 
		//add them to the queue for processing.
		while (!iDevices.isEmpty()) {

			//get next InputDevice
			InputDevice* iDevice = iDevices.pop_front();

			if (iDevice == NULL) {
				continue; //<-- should never happen... but just in case
			}

			//get InputDevice's input Patches
			LinkedList<Patch> patches = iDevice->getInputPatches();
			
			while (!patches.isEmpty()) {
				Patch* patch = patches.pop_front();
				if (patch == NULL) {
					continue; //<--- should also never happen
				}
				queue.push_back(patch);

			}//end patch loop

			//ignore parameters. they are updated implicitly through
			//their owners (Parametrerizables)
			//they are not stored in the order_list
			if (toParameter(iDevice) != NULL) {
				continue;
			}


		}//end input device loop

	}//end queue loop

	changed = false;
}

bool Circuit::process() {

	//check for circuit changes
	if (changed) {
		optimize();
	}
	
	if (!checkInputs()) {
		//inputs not ready
		return false;
	}
	
	
	std::vector<Patch*>::iterator it = firstPatches.begin();
	inputs.apply(getInputs, &it);

	order.apply(processInOrder, NULL);

	it = lastPatches.begin();
	outputs.apply(sendOutputs, &it);
	/*
	//transfer signals from external inputs to "firstPatches"
	LinkedList<Patch> inputs = getInputPatches();
	int index = 0;
	while (!inputs.isEmpty()) {
		double signal = 0.;
		inputs.pop_front()->requestSignal(signal);
		if(index < firstPatches.size())
			firstPatches[index]->pushSignal(signal);
		index++;
	}
	

	//run circuit processing in order
	LinkedList<PatchDevice> order_copy = order;
	while (!order_copy.isEmpty()) {

		PatchDevice *pd = order_copy.pop_front();

		//PatchDevice* pd = current->getInput();

		//test for Parameter
		if (toParameter(pd) != NULL)
			//parameter signals are updated implicitly via 
			//the Effects or Wave Generator's that own them.
			continue;

		if (pd != NULL)
			pd->process();

	}

	//push internal signal to cooresponding output patches
	LinkedList<Patch> outputs = getOutputPatches();
	for (int i = 0; i < lastPatches.size() && !outputs.isEmpty(); i++) {
		Patch* output = outputs.pop_front();
		if (output == NULL) {
			continue; //hit end of list? ...or rogue NULL pointer in output patches???
		}
		if (i < lastPatches.size() && lastPatches[i] != NULL) {
			double signal = 0.;
			if (lastPatches[i]->requestSignal(signal)) {
				output->pushSignal(signal);
			}
		}
	}
	*/
	//success!!!!
	return true;
}

bool Circuit::removeInput(Patch * const patch) {

	LinkedList<Patch> copy = getInputPatches();
	int index = -1;
	bool found = false;

	//find index of patch being removed
	Patch* pop = copy.pop_front();
	while (pop != NULL) {
		index++;
		if (pop == patch) {
			found = true;
			break;
		}
		pop = copy.pop_front();
	}

	if (!found || index == -1) {
		return false;
	}

	if (!InputDevice::removeInput(patch)) {
		//couldn't remove input patch
		//should never happen because the patch was found, but not removed...? How???
		return false;
	}

	//disconnect internal "first patch"
	bool removed = false;
	Patch *remove = firstPatches[index];
	remove->getOutput()->removeInput(remove);
	auto it = firstPatches.begin();
	while (it != firstPatches.end()) {
		if (*it == remove) {
			firstPatches.erase(it);
			removed = true;
			break;
		}
	}
	if (removed) {
		//safely delete the patch
		delete remove;
	}

	changed = true;
	return true;
}

bool Circuit::removeOutput(Patch * const patch) {
	
	LinkedList<Patch> copy = getOutputPatches();
	int index = -1;
	bool found = false;

	//find index of patch being removed
	Patch* pop = copy.pop_front();
	while (pop != NULL) {
		index++;
		if (pop == patch) {
			found = true;
			break;
		}
		pop = copy.pop_front();
	}

	if (!found || index == -1) {
		return false;
	}

	if (!OutputDevice::removeOutput(patch)) {
		//couldn't remove input patch
		//should never happen because the patch was found, but not removed...? How???
		return false;
	}

	//disconnect internal "first patch"
	bool removed = false;
	Patch *remove = lastPatches[index];
	remove->getInput()->removeOutput(remove);
	auto it = lastPatches.begin();
	while (it != lastPatches.end()) {
		if (*it == remove) {
			lastPatches.erase(it);
			removed = true;
			break;
		}
	}
	if (removed) {
		//safely delete the patch
		delete remove;
	}

	changed = true;
	return true;
}

/*
	Remove Patch:
	Removes the Patch from this circuit.
	Takes care of unpatching the input and output devices
	and deletes the patch from the master list
*/
void Circuit::removePatch(Patch * const patch) {
	if (patch->getInput() != NULL) {
		patch->getInput()->removeOutput(patch);
	}
	if (patch->getOutput() != NULL) {
		patch->getOutput()->removeInput(patch);
	}
	patch_master.remove(patch, true);
}

/*
	Helper casting methods:
	Easier and cleaner than writing a bunch of dynamic_casts
*/
InputDevice* Circuit::toInputDevice(PatchDevice* const device) const {
	return dynamic_cast<InputDevice*>(device);
}

Effect* Circuit::toEffect(PatchDevice* const device) const {
	return dynamic_cast<Effect*>(device);
}

WaveGenerator* Circuit::toWaveGenerator(PatchDevice* const device) const {
	return dynamic_cast<WaveGenerator*>(device);
}

OutputDevice* Circuit::toOutputDevice(PatchDevice* const device) const {
	return dynamic_cast<OutputDevice*>(device);
}

Parameterizable* Circuit::toParameterizable(PatchDevice * const device) const {
	return dynamic_cast<Parameterizable*>(device);
}

Timer* Circuit::toTimer(PatchDevice * const device) const {
	return dynamic_cast<Timer*>(device);
}

Circuit* Circuit::toCircuit(PatchDevice * const device) const {
	return dynamic_cast<Circuit*>(device);
}

Parameter* Circuit::toParameter(PatchDevice * const device) const {
	return dynamic_cast<Parameter*>(device);
}



int Circuit::parse(const std::string &str, std::string *&out) const {

	//list to hold indices of device seperaters (':')
	LinkedList<int> indices;

	//find indices of ':'
	for (int index = 0; index < str.length(); index++) {
		if (str[index] == ':') {
			int *i = new int(index);
			indices.push_back(i);
		}
	}

	//filler variable
	int filler = 0;

	//count of substrings
	int strings = indices.getSize() + 1;

	//declare space for string array
	out = new std::string[strings];

	//handle case where there are no devices seperaters
	if (strings == 1) {
		out[0] = str;
		return strings; //(1)
	}

	//start index for current substring
	int* start = new int(0);

	//end index for current substring
	int* end = indices.pop_front();

	//get all substrings
	while (end != NULL) {
		out[filler++] = str.substr(*start, *end);
		delete start;
		start = end;
		++(*start);
		end = indices.pop_front();
	}
	out[filler++] = str.substr(*start);
	delete start;

	//return the size of the array created
	return strings;
}

InputDevice* Circuit::lookupInputDevice(std::string *strings, int size, int &inputChannel) {

	//look for output patching
	if (strings[0] == "out") {
		if (size == 2) {
			//attempt to cast string to an int
			int channel = atoi(strings[1].c_str());

			//if cast returns zero (0), one of two possibilites:
			//		string is "0" or
			//		string could not cast to an int
			//need to test the first char of the string to be sure
			if (channel == 0 && strings[1][0] != '0') {

				//invalid cast
				return NULL;
			}

			//valid cast
			inputChannel = channel;
			return this;
		}
		return NULL;
	}

	//try to find the device
	auto search = devices.find(strings[0]);//store the found device (as iterator)
	if (search == devices.end()) {
		return NULL;
	}

	//if the device is a circuit, and the patch goes into the circuit
	Circuit* circuit = toCircuit(search->second);
	if (circuit != NULL && size > 1) {
		return circuit->lookupInputDevice(&strings[1], size - 1, inputChannel);
	}

	//first device is not a circuit, and there are 2 devices described.
	//the second device must be a parameter
	if (size == 2) {

		//cast first device to parameterizable
		Parameterizable* p = toParameterizable(search->second);

		if (p == NULL) {
			//device was not a circuit or a parameterizable object,
			//then its invalid input
			return NULL;
		}

		Parameter* param = NULL;

		//looks for second device as parameter
		p->getParameter(strings[1], param);
		return param;

	}

	if (size == 1) {

		//return cast to input device
		return toInputDevice(search->second);

	}

	//invalid device description or size
	return NULL;
}

OutputDevice* Circuit::lookupOutputDevice(std::string *strings, int size, int &outputChannel) {

	//look for input patching
	if (strings[0] == "in") {
		if (size == 2) {
			//attempt to cast string to an int
			int channel = atoi(strings[1].c_str());

			//if cast returns zero (0), one of two possibilites:
			//		string is "0" or
			//		string could not cast to an int
			//need to test the first char of the string to be sure
			if (channel == 0 && strings[1][0] != '0') {

				//invalid cast
				return NULL;
			}

			//valid cast
			outputChannel = channel;
			return this;
		}
		return NULL;
	}

	//try to find the device
	auto search = devices.find(strings[0]);//store the found device (as iterator)
	if (search == devices.end()) {
		return NULL;
	}

	//if the device is a circuit, and the patch goes into the circuit
	Circuit* circuit = toCircuit(search->second);
	if (circuit != NULL && size > 1) {
		return circuit->lookupOutputDevice(&strings[1], size - 1, outputChannel);
	}

	//no need to check for parameters since they are inputDevices

	if (size == 1) {

		//return cast to input device
		return toOutputDevice(search->second);

	}

	//invalid device description or size
	return NULL;
}

void Circuit::incrementLevel() {
	//increment my level
	level++;

	//recursively incremtn the levels for all sub-circuits
	for (auto it = devices.begin(); it != devices.end(); ++it) {
		Circuit *c = toCircuit(it->second);
		if (c != NULL) {
			c->incrementLevel();
		}
	}
}

bool Circuit::processInOrder(PatchDevice* pd, void* args) {
	return pd->process();
}

bool Circuit::getInputs(Patch* patch, void* args) {
	std::vector<Patch*>::iterator *it = static_cast<std::vector<Patch*>::iterator *>(args);
	double signal;
	patch->requestSignal(signal);
	(*(*it)++)->pushSignal(signal);
	//it++->operator*->pushSignal(signal);
	return true;
}

bool Circuit::sendOutputs(Patch* patch, void* args) {
	std::vector<Patch*>::iterator* it = static_cast<std::vector<Patch*>::iterator* >(args);
	double signal;
	(*(*it)++)->requestSignal(signal);
	//it++->operator*->requestSignal(signal);
	patch->pushSignal(signal);
	return true;
}

//EOF