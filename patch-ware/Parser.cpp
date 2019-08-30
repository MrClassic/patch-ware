

#include "Parser.h"


void eatUntil(std::ifstream &file, char find){
	char next = 0;
	while (next != find) {
		next = (char)file.get();
	}
}

void eatThese(std::ifstream &file, char find) {
	while (file.peek() == find) {
		file.get();
	}
}

void fillUntil(std::ifstream &file, char find, std::string &ret) {
	std::stringstream buffer;
	while (file.peek() != find){
		buffer << (char)file.get();
	}
	file.get();//eat last character
	ret = buffer.str();
}

PatchDevice* stringToDevice(const std::string &str) {
	PatchDevice* out = NULL;

	// *********** Wave Generators *****************
	WaveProcessor* wave = NULL;
	if (str == "SineWaveGenerator") {
		wave = new SineWaveGenerator;
	}
	else if (str == "SquareWaveGenerator") {
		wave = new SquareWaveGenerator;
	}
	else if (str == "TriangleWaveGenerator") {
		wave = new TriangleWaveGenerator;
	}
	else if (str == "QuadracticWaveGenerator") {
		wave = new QuadraticWaveGenerator;
	}
	else if (str == "ZeroWaveGenerator") {
		wave = new ZeroWaveGenerator;
	}
	else if (str == "SawtoothWaveGenerator") {
		wave = new SawtoothWaveGenerator;
	}

	//********************************************
	//ADD CUSTOM WAVE GENERATOR CONVERSION HERE
	//********************************************
	/* ************************************************************************************
		EXAMPLE:

		CUSTOM_WAVE_GENERATOR					- the name of your custom WaveProcessor
												  implementation class.

	***************************************************************************************
	else if(str == "CUSTOM_WAVE_GENERATOR"){
		wave = new CUSTOM_WAVE_GENERATOR;
	}
	*/

	//custom wave processor handling here!!!


	if (wave != NULL) {
		//if the device is a wave generator we
		//can handle generally
		out = new WaveGenerator(wave);
		return out;
	}

	//***************** Envelopes ******************

	//Envelopes set up the default parameters when created.
	//if any additional parameters are added to the specific
	//implementation, they need to be bound to the envelope
	//parameters manually.
	Envelope* env = NULL;
	if (str == "LinearEnvelope") {
		LinearEnvelope* lEnv = new LinearEnvelope;
		env = new Envelope(lEnv);

		//has no additional parameters to bind, so we're done here!
	}

	//add your custom Envelope class creation here!
	//if your custom Envelope class has no additional parameters
	//it can be created like the LinearEnvelope (using the 
	//Envelope's constructor)
	//if your custom Envelope class does have additional 
	//parameters, set them up similar to how the Effect 
	//parameters are tied together.

	//add your custom Envelope classes here!

	if (env != NULL) {
		out = env;
		return out;
	}


	// *************************** Effects **********************

	//Effects take extra effort as we need to tie the parameters
	//to their memory addresses.
	//(We need to tell the Effect what memory it's
	//Parameters are pointing to within their Signal Processor)
	//if you're confused, try looking it up in the documentation
	//if it exists yet... Or if you know Nate, ask him!
	//Otherwise: good luck!
	if (str == "BiquadFilter") {
		BiquadFilter* biquad = new BiquadFilter;
		Effect* effect = new Effect(biquad);

		//bind parameters
		effect->addParameter("a0", biquad->paramAddr(BiquadFilter::A0));
		effect->addParameter("a1", biquad->paramAddr(BiquadFilter::A1));
		effect->addParameter("a2", biquad->paramAddr(BiquadFilter::A2));
		effect->addParameter("b0", biquad->paramAddr(BiquadFilter::B0));
		effect->addParameter("b1", biquad->paramAddr(BiquadFilter::B1));
		effect->addParameter("b2", biquad->paramAddr(BiquadFilter::B2));

		//set return variable
		out = effect;
	}
	else if (str == "BiquadBandpass") {
		BiquadBandpass* bandpass = new BiquadBandpass;
		Effect* effect = new Effect(bandpass);

		//expose only bandpass parameters
		effect->addParameter("q", bandpass->paramAddr(BiquadBandpass::Q));
		effect->addParameter("fc", bandpass->paramAddr(BiquadBandpass::FC));
		effect->addParameter("peak", bandpass->paramAddr(BiquadBandpass::PEAK));

		out = effect;
	}
	else if (str == "BiquadPeak") {
		BiquadPeak* peak = new BiquadPeak;
		Effect* effect = new Effect(peak);

		//expose only bandpass parameters
		effect->addParameter("q", peak->paramAddr(BiquadBandpass::Q));
		effect->addParameter("fc", peak->paramAddr(BiquadBandpass::FC));
		effect->addParameter("peak", peak->paramAddr(BiquadBandpass::PEAK));

		out = effect;
	}
	else if (str == "BiquadLowpass") {
		BiquadLowpass* lp = new BiquadLowpass;
		Effect* effect = new Effect(lp);

		//expose only bandpass parameters
		effect->addParameter("q", lp->paramAddr(BiquadBandpass::Q));
		effect->addParameter("fc", lp->paramAddr(BiquadBandpass::FC));
		effect->addParameter("peak", lp->paramAddr(BiquadBandpass::PEAK));

		out = effect;
	}
	else if (str == "BiquadHighpass") {
		BiquadHighpass* hp = new BiquadHighpass;
		Effect* effect = new Effect(hp);

		//expose only bandpass parameters
		effect->addParameter("q", hp->paramAddr(BiquadBandpass::Q));
		effect->addParameter("fc", hp->paramAddr(BiquadBandpass::FC));
		effect->addParameter("peak", hp->paramAddr(BiquadBandpass::PEAK));

		out = effect;
	}
	else if (str == "Cicuit") {
		//References another circuit, set a dummy pointer for now,
		//We'll have to attempt to link them together after reading all available
		//circuits from file.
		out = new Circuit;
	}
	else if (str == "Compressor") {
		Compressor * comp = new Compressor;
		Effect* effect = new Effect(comp);

		effect->addParameter("buffer", comp->paramAddr(Compressor::BUFFER));
		effect->addParameter("max", comp->paramAddr(Compressor::MAX));
		effect->addParameter("threshold", comp->paramAddr(Compressor::THRESH));

		out = effect;
	}
	else if (str == "Delay") {
		Delay * delay = new Delay;
		Effect* effect = new Effect(delay);

		effect->addParameter("decay", delay->paramAddr(Delay::DECAY));
		effect->addParameter("mix", delay->paramAddr(Delay::MIX));
		effect->addParameter("regen", delay->paramAddr(Delay::REGEN));

		out = effect;
	}
	else if (str == "Distortion") {
		Distortion * dist = new Distortion;
		Effect* effect = new Effect(dist);

		effect->addParameter("threshold", dist->paramAddr(Distortion::THRESH));

		out = effect;
	}
	else if (str == "FIRFilter") {
		FIRFilter * fir = new FIRFilter;
		Effect* effect = new Effect(fir);

		//binding parameters is dependent on the filter's order
		//and we dont know that yet.
		//do it somewhere else...?

		out = effect;
	}
	else if (str == "Gain") {
		Gain * gain = new Gain;
		Effect* effect = new Effect(gain);

		effect->addParameter("level", gain->paramAddr(Gain::LEVEL));

		out = effect;
	}
	else if (str == "Gate") {
		Gate * gate = new Gate;
		Effect* effect = new Effect(gate);

		effect->addParameter("threshold", gate->paramAddr(Gate::THRESH));

		out = effect;
	}
	else if (str == "IIRFilter") {
		IIRFilter * iir = new IIRFilter;
		Effect* effect = new Effect(iir);

		//binding parameters is dependent on the filter's order
		//and we dont know that yet.
		//do it somewhere else...?

		out = effect;
	}
	else if (str == "InvertDistortion") {
		InvertDistortion * dist = new InvertDistortion;
		Effect* effect = new Effect(dist);

		effect->addParameter("threshold", dist->paramAddr(InvertDistortion::THRESH));

		out = effect;
	}

	//signal spy????

	//********************************************
	//ADD CUSTOM EFFECT PARAMETER BINDING HERE
	//********************************************
	/* ************************************************************************************
		EXAMPLE:

		CUSTOM_EFFECT_NAME						- the name of your custom SignalProcessor 
												  implementation class.

		parameter0, parameter1, parameter2...	- string tags for your Effect's parameters.

		PARAM0, PARAM1, PARAM2...				- integers denoting the parameter's offset into the 
												  signal processor's parameters array.

	***************************************************************************************
	else if(str == "CUSTOM_EFFECT_NAME"){
		CUSTOM_EFFECT_NAME* custom = new CUSTOM_EFFECT_NAME;
		Effect* effect = new Effect(custom);

		effect->addParameter("parameter0", custome->paramAddr(CUSTOM_EFFECT_NAME::PARAM0));
		effect->addParameter("parameter1", custome->paramAddr(CUSTOM_EFFECT_NAME::PARAM1));
		effect->addParameter("parameter2", custome->paramAddr(CUSTOM_EFFECT_NAME::PARAM2));
		//..like that!

		out = effect;
	}
	*/

	//you're custom effects handled here! 

	if (out == NULL) {
		error("XML file parser error: Patch Device: " + str + " not recognized");
	}

	return out;
}

Circuit* fileToCircuit(const std::string &filename) {

	std::ifstream file;
	file.open(filename.c_str());
	if (!file) {
		error("XML file parser error: file: " + filename + " could not be opened");
		return NULL;
	}
	
	//std::stringstream builder;
	
	eatUntil(file, '<');

	std::string circuitName;
	fillUntil(file, ' ', circuitName);
	if (circuitName != "Circuit") {
		error("XML file parser error: Opening tag not identified as 'Circuit'");
		return NULL;
	}

	Circuit* circuit = new Circuit;

	//inside Circuit opening tag
	eatThese(file, ' ');

	//vector to hold circuit descriptors inside opening circuit tag
	std::vector<std::pair<std::string, std::string>> circuitDescriptors;

	char next = 0;

	//extract circuit descriptors
	while (next != '>') {
		std::string first;
		std::string second;

		fillUntil(file, '=', first);
		eatUntil(file, '"');
		fillUntil(file, '"', second);
		eatThese(file, '"');
		eatThese(file, ' ');

		if (first != "" && second != "") {
			circuitDescriptors.push_back(std::pair<std::string, std::string>(first, second));
		}

		eatThese(file, ' ');

		if (file.peek() == '>') {
			next = (char)file.get();
		}
	}
	int input_channels = -1;
	int output_channels = -1;
	for (auto descriptor = circuitDescriptors.begin(); descriptor != circuitDescriptors.end(); ++descriptor) {
		if (descriptor->first == "channels") {
			try {
				if(input_channels == -1)
					input_channels = std::stoi(descriptor->second);
				if(output_channels == -1)
					output_channels = std::stoi(descriptor->second);
			}
			catch (std::exception &e) {
				error("XML file parser error: channels attribute cannot be cast to integer (" + descriptor->second + ')');
				return NULL;
			}
		}
		if (descriptor->first == "input_channels") {
			try {
				input_channels = std::stoi(descriptor->second);
			}
			catch (std::exception &e) {
				error("XML file parser error: input_channel attribute cannot be cast to integer (" + descriptor->second + ')');
				return NULL;
			}
		}
		if (descriptor->first == "output_channels") {
			try {
				if (output_channels == -1)
					output_channels = std::stoi(descriptor->second);
			}
			catch (std::exception &e) {
				error("XML file parser error: output_channel attribute cannot be cast to integer (" + descriptor->second + ')');
				return NULL;
			}
		}
	}

	if (input_channels < 0 || output_channels < 0) {
		error("XML file parser error: Circuit channels must be specified in parent circuit tag");
		return NULL;
	}
	Patch* inputPatches = new Patch[input_channels];
	Patch* outputPatches = new Patch[output_channels];

	for (int channel = 0; channel < input_channels; channel++) {
		circuit->addInput(&inputPatches[channel]);
	}
	for (int channel = 0; channel < output_channels; channel++) {
		circuit->addOutput(&outputPatches[channel]);
	}

	eatUntil(file, '<');

	//vector of pairs of pairs of strings
	std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>> patches;

	//while Patch Devices
	while (file.peek() != '/') {

		//device type
		std::string device;
		fillUntil(file, ' ', device);

		if (device == "Patch") {

			std::pair<std::string, std::string> pair1, pair2;
			eatThese(file, ' ');
			fillUntil(file, '=', pair1.first);
			eatUntil(file, '"');
			fillUntil(file, '"', pair1.second);

			eatThese(file, ' ');
			fillUntil(file, '=', pair2.first);
			eatUntil(file, '"');
			fillUntil(file, '"', pair2.second);

			if (pair1.first == "" || pair1.second == "" ||
				pair2.first == "" || pair2.second == "") {

				error("XML file parser error: invalid patch description encountered\n\t" 
					+ pair1.first + '=' + pair1.second + "\n\t" 
					+ pair2.first + '=' + pair2.second);
				return NULL;

			}

			//add pairs to  the patches vector.
			//process them later
			std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> insert;
			insert.first = pair1;
			insert.second = pair2;
			patches.push_back(insert);

			eatUntil(file, '<');
			eatUntil(file, '<');

			//continue looking for devices or patches
			continue;

		}

		//vector of pairs
		std::vector<std::pair<std::string, std::string>> deviceDescriptors;

		//vector of pairs of strings and vectors of pairs (of strings and strings)
		//dude, thats crazy!!!
		std::vector<std::pair<std::string, std::vector<std::pair<std::string, std::string>>>> tags;

		eatThese(file, ' ');

		
		std::string first;
		std::string second;
		next = 0;

		//get device descriptor(s)

		//while inside Patch Device Tag
		while (next != '>') {

			fillUntil(file, '=', first);
			eatThese(file, ' ');
			eatUntil(file, '"');
			fillUntil(file, '"', second);

			if (first != "" && second != "") {
				deviceDescriptors.push_back(std::pair<std::string, std::string>(first, second));
			}

			eatThese(file, ' ');

			if (file.peek() == '>') {
				next = (char)file.get();
			}

		}

		PatchDevice* patchDevice = stringToDevice(device);

		if (patchDevice == NULL) {
			error("XML file parser error: Illegal device type: " + device);
			return NULL;
		}

		Effect* isEffect = dynamic_cast<Effect*>(patchDevice);
		
		Filter* isFilter = NULL;
		
		if (isEffect) {
			isFilter = dynamic_cast<Filter*>(isEffect->getProc());
		}

		std::string name = "";
		for (auto it = deviceDescriptors.begin(); it != deviceDescriptors.end(); ++it) {
			if (it->first == "name") {
				name = it->second;
			}
			if (it->first == "order") {
				if (!isFilter) {
					error("XML file parser error: Can only specify 'order' for Filter devices (device: " + device + ')');
					return NULL;
				}
				int order;
				try {
					order = std::stoi(it->second);
				}
				catch (std::exception &e) {
					error("XML file parser error: order type (" + it->second + ") could not be cast to an integer");
					return NULL;
				}

				//set filter order
				isFilter->setOrder(order);

				//bind parameters to coefficients
				for (int o = 0; o <= order; ++o) {

					std::stringstream buffer;//to combine "coefficient" and the current coefficient number
					buffer << "coefficient" << o;

					isEffect->addParameter(buffer.str(), isFilter->paramAddr(Filter::COEFFICIENTS + o));

				}
			}
		}
		if (name == "") {

			error("XML file parser error: no device name specified for device type '" + device + "'");
			return NULL;
		}

		

		//add device to circuit
		circuit->addDevice(name, patchDevice);

		eatUntil(file, '<');

		if (file.peek() == '/') {
			//hit the end of the patch device in file
			eatUntil(file, '<');
			continue;
		}

		next = 0;

		//while tags inside patch device tag
		while (next != '<') {

			//get tags inside patch device
			std::string tag;

			fillUntil(file, ' ', tag);
			eatThese(file, ' ');

			if (file.peek() == '>') {
				next = (char)file.get();
				//no attributes inside tag
				continue;
			}

			std::vector<std::pair<std::string, std::string>> pairs;
			next = 0;

			//while inside "variable" tags (param, input_type... etc.)
			while (next != '>') {

				std::string first;
				std::string second;

				fillUntil(file, '=', first);
				eatUntil(file, '"');
				fillUntil(file, '"', second);
				eatThese(file, ' ');

				pairs.push_back(std::pair<std::string, std::string>(first, second));

				if (file.peek() == '>') {
					//tag closed
					file >> next;
					continue;
				}

			}//end "while inside 'variable' tags" loop

			//push this tag into "tags" vector
			tags.push_back(std::pair<std::string, std::vector<std::pair<std::string, std::string>>>(tag, pairs));

			//skip closing tag
			eatUntil(file, '<');
			eatUntil(file, '<');

			if (file.peek() == '/') {
				next = '<';
				continue;
			}
		
		}//end "while tags inside patch device" loop 

		//use tags to modifiy the current patch device

		for (auto tag = tags.begin(); tag != tags.end(); ++tag) {
			std::string tagType = tag->first;
			if (tagType == "param") {
				Parameterizable* currentDevice = dynamic_cast<Parameterizable*>(patchDevice);
				if (currentDevice == NULL) {

					//ERROR <----------------------------------------------------------------------
					//Device does not support parameters
					error("XML file parser error: Device '" + device + "' does not support parameters");
					return NULL;

				}
				std::string name, value;

				//for each pair inside the current tag
				for (auto pair = tag->second.begin(); pair != tag->second.end(); ++pair) {
					if (pair->first == "name") {
						name = pair->second;
						if (name == "regen") {
							int stall = 0;
						}
					}
					else if (pair->first == "value") {
						value = pair->second;
					}
				}

				if (name != "" && value != "") {
					double val;
					//try string to double
					try {
						val = std::stod(value);
					}
					catch (std::exception &e) {
						error("XML file parser error: value (" + value + ") could not be cast to a number");
						return NULL;

					}

					if (!currentDevice->hasParameter(name)) {
						error("XML file parser error: Device (" + device + ") doesn't have the parameter specified (" + name + ")");
						return NULL;

					}

					//set parameter successfully!! Hooray!!
					currentDevice->setParameter(name, val);

				}
				else if (name == "") {
					error("XML file parser error: No 'name' attribute found in <param> tag (Device: " + device + ')');
					return NULL;

				}
				else if (value == "") {
					error("XML file parser error: No 'value' attribute found in <param> tag (Device: " + device + ')');
					return NULL;
				}

			}
			else if (tagType == "input_type") {

				InputDevice* currentDevice = dynamic_cast<InputDevice*>(patchDevice);

				if (currentDevice == NULL) {
					error("XML file parser error: <input_type> tag used in non-InputDevice (Device: " + device + ')');
					return NULL;
				}

				bool found = false;//<---- not used?
				for (auto pair = tag->second.begin(); pair != tag->second.end(); ++pair) {
					if (pair->first == "value") {
						found = true;

						std::string val = pair->second;
						input_type type = input_type::NUM_OF_TYPES;//default invalid state

						//cast val string to input_type
						if (val == "SUM") {
							type = input_type::SUM;
						}
						else if (val == "PRODUCT") {
							type = input_type::PRODUCT;
						}
						else if (val == "AVERAGE") {
							type = input_type::AVERAGE;
						}
						else if (val == "MAX") {
							type = input_type::MAX;
						}
						else if (val == "MIN") {
							type = input_type::MIN;
						}
						else {
							error("XML file parser error: input_type '" + val + "' not recgonized");
							return NULL;

						}

						//set input type
						currentDevice->setInputType(type);

					}
				}
				if (!found) {

					error("XML file parser error: Could't find a value attribute");
					return NULL;

				}
			}
		}

		//skip closing tag (device)
		//eatUntil(file, '<');
		eatUntil(file, '<');

		if (file.peek() == '/') {
			next = (char)file.get();
		}

	}//end "while patch devices to read" loop

	for (auto it = patches.begin(); it != patches.end(); ++it) {

		std::string input, output;

		//set input string
		if (it->first.first == "input") {
			input = it->first.second;
		}
		else if (it->second.first == "input") {
			input = it->second.second;
		}

		//set output string
		if (it->first.first == "output") {
			output = it->first.second;
		}
		else if (it->second.first == "output") {
			output = it->second.second;
		}

		//check strings
		if (input == "" || output == "") {
			error("XML file parser error: invalid patch description.\n\tinput = " + input + "\n\toutput = " + output);
			return NULL;
		}

		//attempt patch
		if (dynamic_cast<Circuit*>(circuit->getDevice(input)) != NULL || 
			dynamic_cast<Circuit*>(circuit->getDevice(output)) != NULL) {
			//patch references a circuit, patch it later
			//patchLater[circuitName] = std::pair<std::string, std::string>(input, output);

		}
		if (!circuit->patch(input, output)) {
			error("XML file parser error: Patch failed.\n\tinput = " + input + "\n\toutput = " + output);
			return NULL;

		}

	}

	//add to global circuit list
	//circuit_master[circuitName] = circuit;

	return circuit;

}


//EOF