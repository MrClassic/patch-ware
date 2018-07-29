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
*/
#include "Circuit.h"

Circuit::Circuit() {

}

Circuit::Circuit(const Circuit &other) {
	//TODO: this
}

OutputDevice* Circuit::getDevice(const std::string tag) {
	if (devices[tag] == NULL) {
		devices.erase(tag);
		return NULL;
	}
	return devices[tag];
}

//attempts to add the device to this circuit with the tag provided.
//If the tag is already in use, false is returned, else true is returned.
bool Circuit::addDevice(const std::string tag, OutputDevice* const device) {
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
//		input = "myWaveGenerator:Frequency"
//will represent the frequency parameter for the wave generator called
//"myWaveGenerator"
bool Circuit::patch(const std::string output, const std::string input) {

	//extension indices
	size_t outIndex = output.find_first_of(':');
	size_t inIndex = input.find_first_of(':');

	if (outIndex != std::string::npos) {
		//output device not allowed to reference a parameter
		return false;
	}
	
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

	std::string param = ""; //parameter extension
	std::string inputDevice = ""; //inputDevice (before ':')
	bool ext = false; //is there an extension present?
	InputDevice* input_ptr = NULL;//pointer to reference the actual input device (may be a parameter)

	if (inIndex != std::string::npos) {
		ext = true;
		param = input.substr(inIndex + 1); //+ 1 to exclude to ':'
	}
	inputDevice = input.substr(0, inIndex);

	//check devices existance within this circuit
	if (devices[output] == NULL) {
		devices.erase(output);
		return false;
	}
	if (devices[inputDevice] == NULL) {
		devices.erase(inputDevice);
		return false;
	}

	//validate input is an InputDevice
	if (ext) {
		if (toWaveGenerator(devices[inputDevice]) != NULL) {
			if (param == "frequency" || param == "amplitude" || param == "phase") {
				//continue
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
				if (param[0] == 'a' || param[0] == 'b') {

					//parse out 'a' or 'b' first character
					std::string num = param.substr(1);

					//number behind the 'a' or 'b'
					int coe = atoi(num.c_str());

					//coe == 0 if atoi failed OR there was a '0' after the 'a' or 'b'
					if (coe != 0) {
						BiquadFilter* biquad = dynamic_cast<BiquadFilter*>(devices[inputDevice]);
						if (param[0] == 'a') {
							if (coe == 1) {
								input_ptr = &(biquad->geta1());
							}
							else if (coe == 2) {
								input_ptr = &(biquad->geta2());
							}
						}
						else {
							if (coe == 1) {
								input_ptr = &(biquad->getb1());
							}
							else if (coe == 2) {
								input_ptr = &(biquad->getb2());
							}
						}
					}
					else {
						//if param[0] == '0', atoi "failed" (returned 0) but the parameter is still valid
						if (param[1] == '0') {
							//handle coefficient 0
							BiquadFilter* biquad = dynamic_cast<BiquadFilter*>(devices[inputDevice]);
							if (param[0] == 'a') {	
								input_ptr = &(biquad->geta0());
							}
							else {
								input_ptr = &(biquad->getb0());
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
				if (param == "threshold" || param == "buffer" || param == "max") {
					Compressor* comp = dynamic_cast<Compressor*>(devices[inputDevice]);
					if (param == "threshold") {
						input_ptr = &(comp->getThreshold());
					}
					else if (param == "buffer") {
						input_ptr = &(comp->getBuffer());
					}
					else if (param == "max") {
						input_ptr = &(comp->getMax());
					}
				}
				else {
					//not a valid parameter
					return false;
				}
				break;

			// ************************ Delay **********************************
			case DELAY:
				if (param == "decay") {
					Delay* delay = dynamic_cast<Delay*>(devices[inputDevice]);
					input_ptr = &(delay->getDecay());
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
				if (param == "threshold") {
					Distortion* dist = dynamic_cast<Distortion*>(devices[inputDevice]);
					input_ptr = &(dist->getThreshold());
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
				if (param[0] == '0' || (atoi(param.c_str()) != 0)) {
					int reg = atoi(param.c_str());
					Filter* filter = dynamic_cast<Filter*>(devices[inputDevice]);
					if (reg < filter->getOrder()) {
						input_ptr = &(filter->getCoefficient(reg));
					}
					else {
						//register out of range
						return false;
					}
				}
				break;

			// ************************** Gain ********************************
			case GAIN:
				if (param == "level") {
					Gain* gain = dynamic_cast<Gain*>(devices[inputDevice]);
					input_ptr = &(gain->getLevel());
				}
				else {
					//level is a Gain's only parameter
					return false;
				}
				break;

			// **************************** Gate *******************************
			case GATE:
				if (param == "threshold") {
					Gate* gate = dynamic_cast<Gate*>(devices[inputDevice]);
					input_ptr = &(gate->getThreshold());
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
	else {
		//no extension, inputDevice references an actual InputDevice
		if (toInputDevice(devices[inputDevice]) == NULL) {
			return false;
		}
		input_ptr = toInputDevice(devices[inputDevice]);
	}

	//all checks out
	Patch* patch = new Patch;
	devices[output]->addOutput(patch);
	input_ptr->addInput(patch);
	patch_master.push_back(patch);

	return true;
}

void Circuit::optimize() {

	//queue for breadth-first reverse traversal of the circuit
	//starts at the end and makes its way to the beginning
	LinkedList<Patch> queue = lastPatches;

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

				// ...Add more Effects as the library grows!

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

}

InputDevice* Circuit::toInputDevice(OutputDevice* const device) {
	return dynamic_cast<InputDevice*>(device);
}

Effect* Circuit::toEffect(OutputDevice* const device) {
	return dynamic_cast<Effect*>(device);
}

WaveGenerator* Circuit::toWaveGenerator(OutputDevice* const device) {
	return dynamic_cast<WaveGenerator*>(device);
}

