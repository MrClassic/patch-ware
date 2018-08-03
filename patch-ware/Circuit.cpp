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
*/
#include "Circuit.h"

Circuit::Circuit() {

}

Circuit::Circuit(const Circuit &other) {
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
	firstPatches.clear();
	lastPatches.clear();
	patch_master.clear(true);
	for (std::pair<std::string, OutputDevice*> p : devices) {
		delete p.second;
		p.second = NULL;
	}
}

OutputDevice* Circuit::getDevice(const std::string &tag) {
	if (devices[tag] == NULL) {
		devices.erase(tag);
		return NULL;
	}
	return devices[tag];
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
		if (toWaveGenerator(p.second) != NULL) {
			WaveGenerator* gen = toWaveGenerator(p.second);
			gen->incrementTime(time);
		}
		if (getEffectType(p.second) == CIRCUIT) {
			Circuit* circuit = dynamic_cast<Circuit*>(p.second);
			circuit->incrementTime(time);
		}
	}
}

//attempts to add the device to this circuit with the tag provided.
//If the tag is already in use, false is returned, else true is returned.
bool Circuit::addDevice(const std::string &tag, OutputDevice* const device) {
	if (devices[tag] == NULL) {
		devices[tag] = device;
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
	
	//the OutputDevice and InputDevice sides of the patch being added
	OutputDevice* o = NULL;
	InputDevice* i = NULL;

	//The input and output channels IF the patch being added is in
	//"firstPatches" or "lastPatches"
	int iChannel = -1;
	int oChannel = -1;

	if (!getIODevices(output, input, o, i, oChannel, iChannel)) {
		return false;
	}

	//valid results from getIODevices()
	if ((o == NULL && oChannel == -1) || (i == NULL && iChannel == -1) ||
		(oChannel > getInputCount() - 1) || (iChannel > getOutputCount() - 1)) {
		//error state, invalid i/o channel
		return false;
	}

	Patch * patch = new Patch;

	//Set the input of the new patch
	if (o != NULL) {

		//not patching to "firstPatches"
		o->addOutput(patch);

	}
	else {
		//patching into "input" channel (firstPatches)

		//make sure there is a slot in the vector for oChannel
		while (firstPatches.size() < oChannel + 1) {
			firstPatches.push_back(NULL);
		}

		patch->setInput(NULL);//input into patch NULL. Input handled explicitly by the Circuit class
		firstPatches[oChannel] = patch;

	}

	//Set the output of the new patch
	if (i != NULL) {

		//not patching to "firstPatches"
		i->addInput(patch);

	}
	else {
		//patching into "output" channel (lastPatches)

		//make sure there is a slot in the vector for oChannel
		while (lastPatches.size() < iChannel + 1) {
			lastPatches.push_back(NULL);
		}

		patch->setOutput(NULL);//input into patch NULL. Input handled explicitly by the Circuit class
		lastPatches[iChannel] = patch;

	}

	patch_master.push_back(patch);
	changed = true;
	return true;

}

//same parameter conventions as patch()
//see patch() function description
bool Circuit::unpatch(const std::string &output, const std::string &input) {
	
	//the OutputDevice and InputDevice sides of the patch being removed
	OutputDevice* o = NULL; 
	InputDevice* i = NULL; 

	//The input and output channels IF the patch being removed is in
	//"firstPatches" or "lastPatches"
	int iChannel = -1; 
	int oChannel = -1; 

	if (!getIODevices(output, input, o, i, oChannel, iChannel)) {
		return false;
	}

	//candidate patches to be the one to remove
	LinkedList<Patch> candidates;

	//find the patch to remove, and disconnect it

	if (o != NULL) {
		//if the patch being removed is associated with an output device,
		//Store all its patches as "candidate" patches for lookup later (if necissary)
		candidates = candidates + o->getOutputPatches();
	}
	else {
		//if removing from "firstPatches," no lookup required, just remove the patch.
		if (firstPatches[oChannel] != NULL) {
			
			//remove the patch
			removePatch(firstPatches[oChannel]);

			//set slot to NULL
			firstPatches[oChannel] = NULL;

			//the circuit has changed
			changed = true;

			//success (...?)
			return true;
		}
	}


	if (i != NULL) {
		//if the patch being removed is associated with an input device,
		//Store all its patches as "candidate" patches for lookup later
		candidates = candidates + i->getInputPatches();
	}
	else {
		//if removing from "lastPatches," no lookup required, just remove the patch.
		if (lastPatches[iChannel] != NULL) {

			//remove the patch
			removePatch(lastPatches[iChannel]);

			//set slot to NULL
			lastPatches[iChannel] = NULL;

			//the circuit has changed
			changed = true;

			//success (...?)
			return true;
		}
	}

	//if we got here, we have candidate patches to look through
	while (!candidates.isEmpty()) {
		Patch* pop = candidates.pop_front();

		//REMEMBER: o is the output device whose output is the input into this patch
		//and i is the input device whose input is the output of this patch
		if (pop != NULL && pop->getInput() == o && pop->getOutput() == i) {
			//found the patch we're looking for!
			//remove it, flag the circuit as changed and return true!
			removePatch(pop);
			changed = true;
			return true;
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

	//map to track which Patches have already been visited
	std::map<Patch*, bool> visited;

	//loop until queue is empty (i.e. the whole circuit has been visited)
	while (!queue.isEmpty()) {

		//input devices to gather next set of patches from
		LinkedList<InputDevice> iDevices;

		//next pop from the queue
		Patch* pop = queue.pop_front();
		if (pop == NULL) {
			break;
		}

		//output device associated with the popped Patch
		OutputDevice* oDevice = pop->getInput();

		//check for output device as Effect
		if (toEffect(oDevice) != NULL) {

			//handle Effect:
			//if the Effect has any patched parameters, they need to be
			//added to the list of InputDevices (iDevices)

			//get Effect type
			EFFECTS type = getEffectType(oDevice);

			//handle Effect types individually
			switch (type) {

				// ********************** Biquad Filter ************************
				case BIQUAD_FILTER:{
					BiquadFilter * biquad = dynamic_cast<BiquadFilter*>(oDevice);
					if (biquad == NULL)
						break;
					if (biquad->geta0().isPatched()) {
						//get patches for a0
						iDevices.push_back(&biquad->geta0());
					}
					if (biquad->geta1().isPatched()) {
						//get patches for a1
						iDevices.push_back(&biquad->geta1());
					}
					if (biquad->geta2().isPatched()) {
						//get patches for a2
						iDevices.push_back(&biquad->geta2());
					}
					if (biquad->getb0().isPatched()) {
						//get patches for b0
						iDevices.push_back(&biquad->getb0());
					}
					if (biquad->getb1().isPatched()) {
						//get patches for b1
						iDevices.push_back(&biquad->getb1());
					}
					if (biquad->getb2().isPatched()) {
						//get patches for b2
						iDevices.push_back(&biquad->getb2());
					}
					break;
				}

				// *********************** Circuit ******************
				case CIRCUIT: {

					//circuit has no paramters (yet...?)

					break;
				}

				// ************************ Compressor ************************
				case COMPRESSOR: {
					Compressor* compressor = dynamic_cast<Compressor*>(oDevice);
					if (compressor == NULL) {
						break;
					}
					if (compressor->getThreshold().isPatched()) {
						//add threshold to input devices
						iDevices.push_back(&compressor->getThreshold());
					}
					if (compressor->getMax().isPatched()) {
						//add max to input devices
						iDevices.push_back(&compressor->getMax());
					}
					if (compressor->getBuffer().isPatched()) {
						//add buffer to input devices
						iDevices.push_back(&compressor->getBuffer());
					}

					break;
				}

				// **************************** Delay **************************
				case DELAY: {
					Delay* delay = dynamic_cast<Delay*>(oDevice);
					if (delay == NULL) {
						break;
					}
					if (delay->getDecay().isPatched()) {
						//add decay parameter to Input devices
						iDevices.push_back(&delay->getDecay());
					}
					break;
				}

				// ******************* Distortion/Invert Distortion ******************
				case DISTORTION:
				case INVERT_DISTORTION: {
					Distortion* dist = dynamic_cast<Distortion*>(oDevice);
					if (dist == NULL) {
						break;
					}
					if (dist->getThreshold().isPatched()) {
						//add threshold as input device
						iDevices.push_back(&dist->getThreshold());
					}
					break;
				}

				// ********************* Simple Filters *************************
				case FIR_FILTER:
				case IIR_FILTER: {
					Filter* filter = dynamic_cast<Filter*>(oDevice);
					if (filter == NULL) {
						break;
					}
					int order = filter->getOrder();
					for (int i = 0; i < order; i++) {
						if (filter->getCoefficient(i).isPatched()) {
							//add patches coefficient parameter as input device
							iDevices.push_back(&filter->getCoefficient(i));
						}
					}
					break;
				}

				// **************************** Gain *******************************
				case GAIN: {
					Gain* gain = dynamic_cast<Gain*>(oDevice);
					if (gain == NULL) {
						break;
					}
					if (gain->getLevel().isPatched()) {
						//add gain's level as input device
						iDevices.push_back(&gain->getLevel());
					}
					break;
				}

				// *************************** Gate ********************************
				case GATE: {
					Gate* gate = dynamic_cast<Gate*>(oDevice);
					if (gate == NULL) {
						break;
					}
					if (gate->getThreshold().isPatched()) {
						//add threshold as input device
						iDevices.push_back(&gate->getThreshold());
					}
					break;
				}

				// ************************* Signal Spy ***************************
				case SIGNAL_SPY: {
					SignalSpy* spy = dynamic_cast<SignalSpy*>(oDevice);
					if (spy == NULL) {
						break;
					}
					break;
				}

				//*************************************************
				// ...Add more Effects as the library grows!
				//*************************************************

				default:
					break;

			}//end switch/case

		}//end Effect handling

		if (toWaveGenerator(oDevice) != NULL) {

			//handle Wave Generator
			WaveGenerator* gen = dynamic_cast<WaveGenerator*>(oDevice);
			if (gen != NULL) {
				if (gen->getAmplitude().isPatched()) {
					//add Amplitude to input devices
					iDevices.push_back(&gen->getAmplitude());
				}
				if (gen->getFrequency().isPatched()) {
					//add frequency to input devices
					iDevices.push_back(&gen->getFrequency());
				}
				if (gen->getPhase().isPatched()) {
					//add phase to input devices
					iDevices.push_back(&gen->getPhase());
				}
			}
		}

		if (toInputDevice(oDevice) != NULL) {

			//hanlde Input Device
			InputDevice* in = dynamic_cast<InputDevice*>(oDevice);
			if (in != NULL && in->getInputCount() > 0) {
				iDevices.push_back(in);
			}
		}

		//gathered all Input Devices into iDevice!
		
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

				if (visited[patch]) {

					//remove the visited patch from the "order" list
					//so the patch can be put at the front
					LinkedList<Patch> copy = order;
					order.clear();
					while (!copy.isEmpty()) {
						Patch* transfer = copy.pop_front();
						if (transfer != NULL) {
							if (transfer != patch) {
								order.push_back(transfer);
							}
							else {
								//don't add "transfer" back into list
								//effectively removing it
							}
						}
					}

				}
				else {

					//push patch to queue ONLY if this is the first visit
					queue.push_back(patch);

					//set as vistied
					visited[patch] = true;

				}

				//add patch to the front of the order list.
				//front of the list == higher priority when 
				//running a signal through the circuit.
				order.push_front(patch);

			}//end patch loop

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

	//transfer signals from external inputs to "firstPatches"
	LinkedList<Patch> inputs = getInputPatches();
	int index = 0;
	while (!inputs.isEmpty()) {
		double signal = 0.;
		inputs.pop_front()->requestSignal(signal);
		firstPatches[index]->pushSignal(signal);
		index++;
	}

	//run circuit processing in order
	LinkedList<Patch> order_copy = order;
	while (!order_copy.isEmpty()) {
		Patch *current = order_copy.pop_front();

		//check for the input into the patch being a wave generator
		OutputDevice* outputDevice = current->getInput();
		if (toWaveGenerator(outputDevice) != NULL) {
			WaveGenerator* gen = toWaveGenerator(outputDevice);
			gen->pushDouble();
		}

		//check for the output of the patch feeding into an Effect
		InputDevice* inputDevice = current->getOutput();
		if (toEffect(inputDevice) != NULL) {
			Effect* effect = toEffect(inputDevice);
			effect->process();
		}

		//parameter signals are updated implicitly via the Effects or Wave Generator's that own them.

	}

	//push internal signal to cooresponding output patches
	LinkedList<Patch> outputs = getOutputPatches();
	for (int i = 0; i < lastPatches.size() && !outputs.isEmpty(); i++) {
		Patch* output = outputs.pop_front();
		if (output == NULL) {
			continue; //hit end of list? ...or rogue NULL pointer in output patches
		}
		if (lastPatches[i] != NULL) {
			double signal = 0.;
			if (lastPatches[i]->requestSignal(signal)) {
				output->pushSignal(signal);
			}
		}
	}

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

void Circuit::removePatch(Patch * const patch) {
	if (patch->getInput() != NULL) {
		patch->getInput()->removeOutput(patch);
	}
	if (patch->getOutput() != NULL) {
		patch->getOutput()->removeInput(patch);
	}
	patch_master.remove(patch, true);
}

bool Circuit::getIODevices(const std::string &output, const std::string &input, 
		OutputDevice* &o, InputDevice* &i, int &oChannel, int &iChannel) {

	//extension indices
	size_t outIndex = output.find_first_of(':');
	size_t inIndex = input.find_first_of(':');

	//convert to all lowercase
	for (char c : input) {
		if (c >= 'A' && c <= 'Z') {
			c += 32;
		}
	}
	for (char c : output) {
		if (c >= 'A' && c <= 'Z') {
			c += 32;
		}
	}

	//input variables
	std::string inputParam = ""; //input parameter extension (after ':')
	std::string inputDevice = ""; //inputDevice (before ':')
	int inputChannel = -1; //possible input channel
	bool inputExtension = false; //is there an extension present for input device?

								  //output variables
	std::string outputParam = ""; //output parameter extension 
	std::string outputDevice = ""; //outputDevice
	int outputChannel = -1; //possible output channel
	bool outputExtension = false; //is there an extension present for output device?

								  //extract extensions and set up device names
	if (inIndex != std::string::npos) {//extract input extension
		inputExtension = true;
		inputParam = input.substr(inIndex + 1); //+ 1 to exclude to ':'
	}
	inputDevice = input.substr(0, inIndex);

	if (outIndex != std::string::npos) {//extract output extension
		outputExtension = true;
		outputParam = output.substr(outIndex + 1);
	}
	outputDevice = output.substr(0, outIndex);

	if ((outputDevice == "in" && outIndex == std::string::npos) ||
		(inputDevice == "out" && inIndex == std::string::npos)) {
		//either "in" or "out" did not contain channels. Invalid input
		return false;
	}

	//check devices existance within this circuit
	if (outputDevice != "in" && devices[outputDevice] == NULL) {
		devices.erase(outputDevice);//erase the reference we made by checking for existance
		return false;
	}
	if (inputDevice != "out" && devices[inputDevice] == NULL) {
		devices.erase(inputDevice);//erase the reference we made by checking for existance
		return false;
	}

	//validate input is an InputDevice
	if (inputExtension && inputDevice != "out") {
		if (toWaveGenerator(devices[inputDevice]) != NULL) {
			if (inputParam == "frequency" || inputParam == "amplitude" || inputParam == "phase") {
				WaveGenerator* gen = toWaveGenerator(devices[inputDevice]);
				if (inputParam == "frequency") {
					i = &gen->getFrequency();
				}
				else if (inputParam == "amplitude") {
					i = &gen->getAmplitude();
				}
				else if (inputParam == "phase") {
					i = &gen->getPhase();
				}
			}
			else {
				//back out, invalid parameter for a wave generator
				return false;
			}
		}
		else if (toEffect(devices[inputDevice]) != NULL) {

			EFFECTS type = getEffectType(devices[inputDevice]);
			if (type == NUM_OF_EFFECTS) {
				return false; //Effect type not found
			}
			switch (type) {

				// ***************************** Biquad Filter ******************************
			case BIQUAD_FILTER:
				//validate the 'a' or 'b' in parameter extension
				if (inputParam[0] == 'a' || inputParam[0] == 'b') {

					//parse out 'a' or 'b' first character
					std::string num = inputParam.substr(1);

					//number behind the 'a' or 'b'
					int coe = atoi(num.c_str());

					//coe == 0 if atoi failed OR there was a '0' after the 'a' or 'b'
					if (coe != 0) {
						BiquadFilter* biquad = dynamic_cast<BiquadFilter*>(devices[inputDevice]);
						if (inputParam[0] == 'a') {
							if (coe == 1) {
								i = &(biquad->geta1());
							}
							else if (coe == 2) {
								i = &(biquad->geta2());
							}
						}
						else {
							if (coe == 1) {
								i = &(biquad->getb1());
							}
							else if (coe == 2) {
								i = &(biquad->getb2());
							}
						}
					}
					else {
						//if param[0] == '0', atoi "failed" (returned 0) but the parameter is still valid
						if (inputParam[1] == '0') {
							//handle coefficient 0
							BiquadFilter* biquad = dynamic_cast<BiquadFilter*>(devices[inputDevice]);
							if (inputParam[0] == 'a') {
								i = &(biquad->geta0());
							}
							else {
								i = &(biquad->getb0());
							}
						}
						else {
							//not a valid coefficient referenced by the extension
							return false;
						}
					}
				}
				else {
					//first char of extension not 'a' or 'b' means extension
					//is invalid
					return false;
				}
				break;

				// ************************ Circuit **********************************
			case CIRCUIT:
				//no parameters for a circuit
				return false;
				break;

				// *************************** Compressor ******************************
			case COMPRESSOR:
				if (inputParam == "threshold" || inputParam == "buffer" || inputParam == "max") {
					Compressor* comp = dynamic_cast<Compressor*>(devices[inputDevice]);
					if (inputParam == "threshold") {
						i = &(comp->getThreshold());
					}
					else if (inputParam == "buffer") {
						i = &(comp->getBuffer());
					}
					else if (inputParam == "max") {
						i = &(comp->getMax());
					}
				}
				else {
					//not a valid parameter
					return false;
				}
				break;

				// ************************ Delay **********************************
			case DELAY:
				if (inputParam == "decay") {
					Delay* delay = dynamic_cast<Delay*>(devices[inputDevice]);
					i = &(delay->getDecay());
				}
				else {
					//Delay's only valid parameter (right now) is decay
					return false;
				}
				break;

				//************* Distortion and InvertDistortion have the same **********
				//********************* interface handle together **********************
			case DISTORTION:
			case INVERT_DISTORTION:
				if (inputParam == "threshold") {
					Distortion* dist = dynamic_cast<Distortion*>(devices[inputDevice]);
					i = &(dist->getThreshold());
				}
				else {
					//Distrotion's only valid parameter is threshold
					return false;
				}
				break;

				//*********** FIR and IIR filters have the same interface ******************
				//************************ handle together *********************************
			case FIR_FILTER:
			case IIR_FILTER:
				if (inputParam[0] == '0' || (atoi(inputParam.c_str()) != 0)) {
					int reg = atoi(inputParam.c_str());
					Filter* filter = dynamic_cast<Filter*>(devices[inputDevice]);
					if (reg < filter->getOrder()) {
						i = &(filter->getCoefficient(reg));
					}
					else {
						//register out of range
						return false;
					}
				}
				break;

				// ************************** Gain ********************************
			case GAIN:
				if (inputParam == "level") {
					Gain* gain = dynamic_cast<Gain*>(devices[inputDevice]);
					i = &(gain->getLevel());
				}
				else {
					//level is a Gain's only parameter
					return false;
				}
				break;

				// **************************** Gate *******************************
			case GATE:
				if (inputParam == "threshold") {
					Gate* gate = dynamic_cast<Gate*>(devices[inputDevice]);
					i = &(gate->getThreshold());
				}
				else {
					//threshold is the only valid parameter for a Gate
					return false;
				}
				break;

				// *************************** Signal Spy ***************************
			case SIGNAL_SPY:
				//signal spy has no parameters
				return false;
				break;

				//***************************************
				//... add new Effects as they are made
				//***************************************

			default:
				//Effect not found
				return false;
			}
		}
	}
	else if (!inputExtension) {
		//no extension, inputDevice references an actual InputDevice
		if (toInputDevice(devices[inputDevice]) == NULL) {
			return false;
		}
		i = toInputDevice(devices[inputDevice]);
	}
	else {//inputDevice == "out"
		inputChannel = atoi(inputParam.c_str());
		if (inputChannel == 0 && inputParam[0] != '0') {
			//inputParam is NaN
			return false;
		}
		if (inputChannel < 0 || inputChannel > getOutputCount()) {
			//requested channel is invalid
			return false;
		}
		iChannel = inputChannel; //set referenced iChannel to the input channel
	}

	if (outputDevice == "in") {
		outputChannel = atoi(outputParam.c_str());
		if (outputChannel == 0 && outputParam[0] != '0') {
			//outputParam is NaN
			return false;
		}
		if (outputChannel < 0 || outputChannel > getInputCount()) {
			//requested channel is not valid
			return false;
		}
		oChannel = outputChannel; //set the feferenced oChannel to the output channel
	}


	//all checks out
	Patch* patch = new Patch;

	//set the OutputDevice
	if (outputDevice != "in") {
		o = devices[outputDevice];
	}
	else {
		//if "output" == "in", set o = NULL
		o = NULL;
	}
	
	//if "input" == "out", set i = NULL
	if (inputDevice == "out") {
		i = NULL;
	}

	return true;
}

InputDevice* Circuit::toInputDevice(OutputDevice* const device) const {
	return dynamic_cast<InputDevice*>(device);
}

Effect* Circuit::toEffect(OutputDevice* const device) const {
	return dynamic_cast<Effect*>(device);
}

WaveGenerator* Circuit::toWaveGenerator(OutputDevice* const device) const {
	return dynamic_cast<WaveGenerator*>(device);
}

OutputDevice* Circuit::toOutputDevice(InputDevice* const device) const {
	return dynamic_cast<OutputDevice*>(device);
}

Effect* Circuit::toEffect(InputDevice* const device) const {
	return dynamic_cast<Effect*>(device);
}

Parameter* Circuit::toParameter(InputDevice* const device) const {
	return dynamic_cast<Parameter*>(device);
}

EFFECTS getEffectType(OutputDevice* device) {

	if (dynamic_cast<BiquadFilter*>(device) != NULL) {
		return BIQUAD_FILTER;
	}
	if (dynamic_cast<Circuit*>(device) != NULL) {
		return CIRCUIT;
	}
	if (dynamic_cast<Compressor*>(device) != NULL) {
		return COMPRESSOR;
	}
	if (dynamic_cast<Delay*>(device) != NULL) {
		return DELAY;
	}
	if (dynamic_cast<Distortion*>(device) != NULL) {
		return DISTORTION;
	}
	if (dynamic_cast<FIRFilter*>(device) != NULL) {
		return FIR_FILTER;
	}
	if (dynamic_cast<Gain*>(device) != NULL) {
		return GAIN;
	}
	if (dynamic_cast<Gate*>(device) != NULL) {
		return GATE;
	}
	if (dynamic_cast<IIRFilter*>(device) != NULL) {
		return IIR_FILTER;
	}
	if (dynamic_cast<InvertDistortion*>(device) != NULL) {
		return INVERT_DISTORTION;
	}
	if (dynamic_cast<SignalSpy*>(device) != NULL) {
		return SIGNAL_SPY;
	}

	//... insert new Effects as they are made

	return NUM_OF_EFFECTS; //error code, not an Effect
}