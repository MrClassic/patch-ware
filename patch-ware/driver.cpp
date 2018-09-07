/* *************************************************************************
 * File:    driver.cpp
 * Author:  Nate Gallegos
 * Log
 *      5/4/17
 *      File Created
 ************************************************************************* */

//#include <sys/ioctl.h>
//#include <unistd.h>
//#include <linux/kd.h>
#include <fstream>
#include <string>
#include <exception>
//#include <fftw.h>
#include <chrono>
#include "RtAudio.h"

#include "FIRFilter.h"
#include "IIRFilter.h"
#include "SineWaveGenerator.h"
#include "SquareWaveGenerator.h"
#include "TriangleWaveGenerator.h"
#include "SawtoothWaveGenerator.h"
#include "QuadraticWaveGenerator.h"
#include "Compressor.h"
#include "Distortion.h"
#include "InvertDistortion.h"
#include "Delay.h"
#include "BiquadFilter.h"
#include "Gain.h"
#include "ZeroWaveGenerator.h"
#include "SignalSpy.h"
#include "Circuit.h"
#include "PulseGenerator.h"
#include "LinearEnvelope.h"

/*
void playSound(){
    int freq[] = { // C   D    E    F    G    A    B    C 
                    523, 587, 659, 698, 784, 880, 988, 1046 };
    int i;
	
    for (i=0; i<8; i++)
    {
            ioctl(STDOUT_FILENO, KIOCSOUND, 1193180/freq[i]);
            usleep(500000);
    }
    ioctl(STDOUT_FILENO, KIOCSOUND, 0); //Stop silly sound

}
*/

void print(double in){
    std::cout << in;
}

void println(double in){
    std::cout << in << std::endl;
}
void print(std::string in){
    std::cout << in;
}

void println(std::string in){
    std::cout << in << std::endl;
}

void print(Filter &in){
    std::cout << "Printing Registers:\n";
    in.printRegisters(std::cout);
    
    std::cout << "\nPrinting Coefficients:\n";
    in.printCoefficients(std::cout);
    
    std::cout << "\n";
}

void showPush(Filter &in, double p){
    print(in);
    print("pushing ");
    println(p);
    print("output: ");
    println(in.pushDouble(p));
    println("");
    
}
void filterTester(){
    Filter *filter;
    filter = new FIRFilter;
    filter->setOrder(3);
    filter->setCoefficient(0, 0.1);
    filter->setCoefficient(1, 0.2);
    filter->setCoefficient(2, 0.3);
    filter->setCoefficient(3, 0.4);
    filter->setBypass(false);
    double push = 0.01;
    for(int i = 0; i < 15; i++){
        push += 0.01 * (double)i;
        showPush(*filter, push);
    }
    
    delete filter;
    filter = new IIRFilter;
    filter->setOrder(3);
    filter->setCoefficient(0, -0.1);
    filter->setCoefficient(1, -0.2);
    filter->setCoefficient(2, -0.3);
    filter->setCoefficient(3, -0.4);
    filter->setBypass(false);
    push = 0.01;
    for(int i = 0; i < 15; i++){
        push += 0.01 * (double)i;
        showPush(*filter, push);
    }
    
    delete filter;
    
}
/*
void waveTester(){
    WaveGenerator *waveMaker;
    
    double frequency = 2;
    waveMaker = new TriangleWaveGenerator;
    waveMaker->setAmplitude(1);
    waveMaker->setFrequency(frequency);
    for(int i = 0; i < 62; i++){
        println(waveMaker->pushDouble());
        waveMaker->incrementTime(1.0/(60.0));
    }
    delete waveMaker;
    
}

void compressorTester(){
    Compressor *comp;
    comp = new Compressor;
    comp->setBuffer(0.5);
    comp->setThreshold(0.75);
    comp->setMax(1.0);
    
    double signal = 0.0;
    for(int i = 0; i < 10; i++){
        signal += 0.1;
        print("Signal:\t\t");
        println(signal);
        print("Compressed:\t");
        println(comp->process(signal));
    }
    
    delete comp;
}

void distortionTester(){
    Distortion *distortion = new InvertDistortion;
    distortion->setThreshold(0.5);
    double signal = 0.0;
    for(int i = 0; i < 10; i++){
        signal += 0.1;
        print("Signal: ");
        println(signal);
        print("Out: \t");
        println(distortion->process(signal));
    }
    
    delete distortion;
}

void delayTester(){
    Delay *delay = new Delay;
    delay->setDecay(0.2);
    delay->setMemory(5);
    
    double signal = 0.0;
    for(int i = 0; i < 5; i++){
        signal += 0.1;
        print("Signal:\t");
        println(signal);
        print("Output:\t");
        println(delay->process(signal));
    }
    for(int i = 0; i < 10; i++){
        print("Signal:\t");
        println(0.0);
        print("Output:\t");
        println(delay->process(0.0));
    }
    
}

void gainTester(){
    
    Gain *gain = new Gain;
    gain->setBypass(false);
    gain->setLevel(0.5);
    double signal = 0.0;
    for(int i = 0; i < 5; i++){
        signal += 0.1;
        print("Signal:\t");
        println(signal);
        print("Output:\t");
        println(gain->process(signal));
    }
    delete gain;
}

void biquadTester(){
    BiquadFilter* filter = new BiquadFilter;
    filter->setBypass(false);
    filter->seta0(1.0);
    filter->seta1(0.25);
    filter->seta2(0.1);
    filter->setb0(1.0);
    filter->setb1(0.75);
    filter->setb2(0.5);
    double signal = 0.0;
    for(int i = 0; i < 5; i++){
        signal += 0.1;
        print("Signal:\t");
        println(signal);
        print("Output:\t");
        println(filter->process(signal));
    }
    for(int i = 0; i < 15; i++){
        print("Signal:\t");
        println(0.0);
        print("Output:\t");
        println(filter->process(0.0));
    }
    delete filter;
}
*/

/* **********************************************************************
 * Patch Devices: creates a patch and connects uses it to connect the
 * output of the output device to the input of the input device. The patch
 * is created dynamically and it's address is returned via a Patch pointer.
 * The user of this function is responsible for catching and managing the 
 * returned Patch pointer.
 * Pre-conditions: Both Output and Input devices should be valid pointers
 * to valid instantiated devices.
 * Post-conditions: The output and input devices will be patched together,
 * and the newly created Patch will be returned as a pointer.
 ********************************************************************** */
Patch* patchDevices(OutputDevice* out, InputDevice * in){
    Patch *p = new Patch();
    out->addOutput(p);
    in->addInput(p);
    return p;
}

void patchDriverCopy(){
    
    //master lists
    LinkedList<Patch> patchMaster;
    LinkedList<Effect> effectMaster;
    LinkedList<WaveGenerator> genMaster;
    
    double phase = 0.;
    double amp = 1.;
    double freq = 1.;
    
    //wave generator for parameter controls
    WaveGenerator *param = new SineWaveGenerator();
    param->setPhase(0.);
    param->setAmplitude(2);
    param->setFrequency(1.7);
    
    //wave generator for signal boosting
    WaveGenerator *boost = new ZeroWaveGenerator(4.00001);
    
    //sine wave generator for signal
    WaveGenerator *signal = new SquareWaveGenerator();
    signal->setAmplitude(1.);
    signal->setFrequency(8.0);
    signal->setPhase(0.);
    
    //add to master lists
    genMaster.push_back(param);
    genMaster.push_back(boost);
    genMaster.push_back(signal);
    
    //Gain effect
    Gain* gain = new Gain();
	gain->setParameter("level", 1.0);
    //gain->setLevel(1.0);
    gain->setBypass(false);
    gain->setInputType(SUM);
    effectMaster.push_back(gain);
    
    std::ofstream out;
    out.open("output.csv"); //circuit output file
    std::ofstream parameter;
    parameter.open("param.csv"); //parameter spy output file
    
    SignalSpy* outSpy = new SignalSpy(out); //spy for output
    effectMaster.push_back(outSpy);
    
    SignalSpy* paramSpy = new SignalSpy(parameter); //spy for parameter
    effectMaster.push_back(paramSpy);
    
    patchMaster.push_back(patchDevices(param, gain));
    patchMaster.push_back(patchDevices(boost, gain));
    patchMaster.push_back(patchDevices(gain, paramSpy));
	;; patchMaster.push_back(patchDevices(paramSpy, &signal->getFrequency()));
    patchMaster.push_back(patchDevices(signal, outSpy));
    
    
    //loop conditions
    const double deltaT = 0.001;
    const double seconds = 1;
    
    //time simulator
    for(int time = 0; time < seconds/deltaT; time++){
        
        //shallow copy lists
        LinkedList<WaveGenerator> gens = genMaster;
        LinkedList<Effect> fx = effectMaster;
    
        //process generators and effects
        int genPops = 0;
        int fxPops = 0;
        while( (!gens.isEmpty() || !fx.isEmpty()) && (genPops < gens.getSize() || fxPops < fx.getSize()) ){
            WaveGenerator* genPop = gens.pop_front();
            if(genPop != NULL){
                if(genPop->paramsReady()){
                    if(genPop == signal){
                        int stall = 0;
                        LinkedList<Patch> patches = patchMaster;
                        while(!patches.isEmpty()){
                            Patch* pop = patches.pop_front();
                            if(pop != NULL && pop->getOutput() == &signal->getFrequency() && *pop){
                                double signal = 0;
                                pop->requestSignal(signal);
                                println(signal);
                                pop->pushSignal(signal);
                            }
                        }
                    }
                    genPop->incrementTime(deltaT);
                    genPop->process();
                    genPops = 0;
                }
                else{
                    genPops++;
                    gens.push_back(genPop);
                }
            }
            Effect* fxPop = fx.pop_front();
            if(fxPop != NULL){
                if(fxPop == paramSpy){
                    int stall = 0;
                }
                if(!fxPop->process()){
                    fx.push_back(fxPop);
                    fxPops++;
                }
                else{
                    fxPops = 0;
                }
            }
        }
        
    }
    
    //close ofstreams
    out.close();
    parameter.close();
    
    //clean up memory
    genMaster.clear(true);
    effectMaster.clear(true);
    patchMaster.clear(true);
}
void patchDriver(){
    
    //master lists
    LinkedList<Patch> patchMaster;
    LinkedList<Effect> effectMaster;
    LinkedList<WaveGenerator> genMaster;
    
    double phase = 0.;
    double amp = 1.;
    double freq = 1.;
    
    //wave generator for parameter controls
    WaveGenerator *param = new SineWaveGenerator();
    param->setPhase(0.);
    param->setAmplitude(2);
    param->setFrequency(1.7);
    
    //wave generator for signal boosting
    WaveGenerator *boost = new ZeroWaveGenerator(4.00001);
    
    //sine wave generator for signal
    WaveGenerator *signal = new SquareWaveGenerator();
    signal->setAmplitude(1.);
    signal->setFrequency(8.0);
    signal->setPhase(0.);
    
    //add to master lists
    genMaster.push_back(param);
    genMaster.push_back(boost);
    genMaster.push_back(signal);
    
    //Gain effect
    Gain* gain = new Gain();
    gain->setParameter("level", 1.0);
    gain->setBypass(false);
    gain->setInputType(SUM);
    effectMaster.push_back(gain);
    
    std::ofstream out;
    out.open("output.csv"); //circuit output file
    std::ofstream parameter;
    parameter.open("param.csv"); //parameter spy output file
    
    SignalSpy* outSpy = new SignalSpy(out); //spy for output
    effectMaster.push_back(outSpy);
    
    SignalSpy* paramSpy = new SignalSpy(parameter); //spy for parameter
    effectMaster.push_back(paramSpy);
    
    patchMaster.push_back(patchDevices(param, gain));
    patchMaster.push_back(patchDevices(boost, gain));
    patchMaster.push_back(patchDevices(gain, paramSpy));
    patchMaster.push_back(patchDevices(paramSpy, &signal->getAmplitude()));
    patchMaster.push_back(patchDevices(signal, outSpy));
    
    //loop conditions
    const double deltaT = 0.001;
    const double seconds = 1;
    
    //time simulator
    for(int time = 0; time < seconds/deltaT; time++){
        
        //shallow copy lists
        LinkedList<WaveGenerator> gens = genMaster;
        LinkedList<Effect> fx = effectMaster;
    
        //process generators and effects
        int genPops = 0;
        int fxPops = 0;
        while( (!gens.isEmpty() || !fx.isEmpty()) && (genPops < gens.getSize() || fxPops < fx.getSize()) ){
            WaveGenerator* genPop = gens.pop_front();
            if(genPop != NULL){
                if(genPop->paramsReady()){
                    if(genPop == signal){
                        int stall = 0;
                        LinkedList<Patch> patches = patchMaster;
                        while(!patches.isEmpty()){
                            Patch* pop = patches.pop_front();
                            if(pop != NULL && pop->getOutput() == &signal->getFrequency() && *pop){
                                double signal = 0;
                                pop->requestSignal(signal);
                                println(signal);
                                pop->pushSignal(signal);
                            }
                        }
                    }
                    genPop->incrementTime(deltaT);
                    genPop->process();
                    genPops = 0;
                }
                else{
                    genPops++;
                    gens.push_back(genPop);
                }
            }
            Effect* fxPop = fx.pop_front();
            if(fxPop != NULL){
                if(fxPop == paramSpy){
                    int stall = 0;
                }
                if(!fxPop->process()){
                    fx.push_back(fxPop);
                    fxPops++;
                }
                else{
                    fxPops = 0;
                }
            }
        }
        
    }
    
    //close ofstreams
    out.close();
    parameter.close();
    
    //clean up memory
    genMaster.clear(true);
    effectMaster.clear(true);
    patchMaster.clear(true);
}

void sineTest(){
    double deltaT = 0.01;
    for(double i = 0.; i < 1; i += deltaT){
        print("Sine(");
        print(i * 360);
        print(") = ");
        double sin = sineD(i * 360.);
        println(sin);
        print("\tArc Sine(");
        print(sin);
        print(") = ");
        println(asineD(sin));
    }
}

void circuitTest() {

	//patches for linking into Circuit
	Patch inLink;
	Patch outLink;

	
	//circuit for testing
	Circuit circuit;

	//******** Circuit internal devices *************

	//wave generator to control signal's frequency
	SineWaveGenerator* pulse = new SineWaveGenerator;
	pulse->setAmplitude(1.);
	pulse->setFrequency(16.);
	pulse->setPhase(0.);
	circuit.addDevice("pulse", pulse);
	
	//signal for the circuit, simple sine wave
	PulseGenerator* signalIn = new PulseGenerator;
	signalIn->setAmplitude(1.);
	signalIn->setFrequency(2.0);
	signalIn->setPhase(0.0);
	signalIn->addOutput(&inLink);

	//zero wave generator to sum frequency with gain
	ZeroWaveGenerator* zero = new ZeroWaveGenerator;
	zero->setAmplitude(0.);
	circuit.addDevice("zero", zero);

	//Gain
	Gain *gain = new Gain;
	gain->setParameter("level", 1.);
	gain->setInputType(input_type::SUM);
	circuit.addDevice("gain", gain);
	

	//FIR filter
	Filter *fir = new IIRFilter;
	//fir->setBypass(true);
	fir->setOrder(10);
	//set up coefficients
	fir->setCoefficient(0, 1.);
	fir->setCoefficient(1, -0.08);
	fir->setCoefficient(2, -0.04);
	fir->setCoefficient(3, -0.03);
	fir->setCoefficient(4, 0.035);
	fir->setCoefficient(5, -0.025);
	fir->setCoefficient(6, -0.625);
	fir->setCoefficient(7, 0.125);
	fir->setCoefficient(8, 0.025);
	fir->setCoefficient(9, -.15);
	fir->setCoefficient(10, -0.1);
	circuit.addDevice("fir", fir);
	
	//Signal Spy
	std::ofstream file("rawOut.csv");
	SignalSpy* rawSpy = new SignalSpy(file);
	circuit.addDevice("rawSpy", rawSpy);

	//Patch the internals of the circuit
	circuit.addInput(&inLink);
	circuit.addOutput(&outLink);
	circuit.patch("in:0", "fir");
	//circuit.patch("zero", "gain");
	//circuit.patch("gain", "pulse:frequency");
	//circuit.patch("pulse", "fir");
	circuit.patch("fir", "rawSpy");
	circuit.patch("rawSpy", "out:0");
	

	//time simulator
	const double MAX_TIME = .05;
	const double STEP = 0.001;
	for (double t = 0.0; t < MAX_TIME; t += STEP) {

		signalIn->process();
		circuit.process();
		double signal = 0.;
		if (!outLink.requestSignal(signal)) {
			std::cout << "Something bad happened: time = " << t << '\n';
			system("pause");
		}
		circuit.incrementTime(STEP);
		signalIn->incrementTime(STEP);
	}
	delete signalIn;
	file.close();
	int stall = 0;
}

void envelopeTest() {

	//patches for linking into Circuit
	Patch inLink;
	Patch outLink;


	//circuit for testing
	Circuit circuit;
	circuit.addOutput(&outLink);
	circuit.addInput(&inLink);

	//sine wave as signal into circuit
	SineWaveGenerator* signalIn = new SineWaveGenerator;
	signalIn->setAmplitude(1.);
	signalIn->setFrequency(1.);
	signalIn->setPhase(0.);
	signalIn->addOutput(&inLink);

	Gain* gain = new Gain;
	gain->setParameter("level", 1.0);
	circuit.addDevice("gain", gain);

	//Envelope
	Envelope* env = new LinearEnvelope;
	env->setParameter("threshold", 1.0);
	env->setParameter("duration", .25);
	env->setParameter("base", 0.);
	circuit.addDevice("envelope", env);

	//Signal Spy
	std::ofstream file("out.csv");
	SignalSpy* rawSpy = new SignalSpy(file);
	circuit.addDevice("spy", rawSpy);
	
	circuit.patch("in:0", "gain");
	circuit.patch("gain", "envelope");
	circuit.patch("gain", "spy");
	circuit.patch("envelope", "spy");
	circuit.patch("spy", "out:0");

	//time simulator
	const double MAX_TIME = 1.;
	const double STEP = 0.01;
	for (double t = 0.0; t < MAX_TIME; t += STEP) {

		signalIn->process();
		circuit.process();
		double signal = 0.;
		if (!outLink.requestSignal(signal)) {
			std::cout << "Something bad happened: time = " << t << '\n';
			system("pause");
		}
		circuit.incrementTime(STEP);
		signalIn->incrementTime(STEP);
	}
	delete signalIn;
	file.close();
	int stall = 0;

}

struct MyData{
	unsigned int channels;
	unsigned int frameRate;
	unsigned int frameSize;
	Patch** input;
	Patch** output;
	Circuit* device;
	bool keepGoing;
};

int call_back(
		void* outputBuffer,			//output buffer
		void* inputBuffer,			//input buffer
		unsigned int nFrames,		//buffer size
		double streamTime,			//time since stream opened
		RtAudioStreamStatus status, //stream status (ignored)
		void* userData) {			//my data being passed in
	//********************************************************

	//auto start = std::chrono::system_clock::now();

	//retrieve my data pointer
	MyData* myData = (MyData*)userData;
	
	//check for stopping signal
	if (!myData->keepGoing) {
		//clear buffers on return
		return 1;
	}

	unsigned int index = 0;
	unsigned int outdex = 0;

	double* iBuf = (double*)inputBuffer;
	double* oBuf = (double*)outputBuffer;
	try {
		//while still stuff to read from buffer
		for (int sample = 0; sample < nFrames; sample++) {
			

			//bypass
			//for (int channel = 0; channel < myData->channels; channel++) {
			//	oBuf[outdex++] = iBuf[index++];
			//}

			//load input(s) into patches
			for (int channel = 0; channel < myData->channels; channel++) {
				if (!myData->input[channel]->pushSignal(iBuf[index++])) {
					std::string message = "Push signal failed. channel: " + std::to_string(channel) + '\n';
					throw std::exception(message.c_str());
				}
				//std::cout << "input[" << index - 1 << "] = " << iBuf[index - 1] << '\n';
			}

			//process the circuit
			if (!myData->device->process())
				throw std::exception("Patch Device could not process");

			//increment time
			myData->device->incrementTime(1. / (double)myData->frameRate);

			//load output(s) from patches
			for (int channel = 0; channel < myData->channels; channel++) {
				if (!myData->output[channel]->requestSignal(oBuf[outdex++])) {
					std::string message = "Request signal failed. channel: " + std::to_string(channel) + '\n';
					throw std::exception(message.c_str());
				}
				//std::cout << "output[" << outdex - 1 << "] = " << oBuf[outdex - 1] << '\n';
			}
			
		}

	}
	catch (std::exception& e) {
		std::cerr << "Exception caught: " << e.what() << '\n'
			<< " RT Audio status code: " << status << '\n';
		return 2;
	}

	//auto end = std::chrono::system_clock::now();
	//std::chrono::duration<double> duration = end - start;

	//std::cout << "Processing: " << myData->channels << " channels, " << nFrames << " frames...\n  " << duration.count() << " seconds\n";

	return 0;
}

void rtAudioTest() {

	//number of channels
	const unsigned int CHANNELS = 1;
	unsigned int bufferFrames = 512;

	//input patches for interfacing with RT Audio
	Patch** inputs = new Patch*[CHANNELS];

	//output patches for interfacing with RT Audio
	Patch** outputs = new Patch*[CHANNELS];

	//master circuit
	Circuit* master = new Circuit;

	//internal device for master circuit
	//Gain g0;
	//g0.setParameter("level", 1.);
	//master->addDevice("g0", g0);

	//Gain *g1 = new Gain;
	//g1->setParameter("level", 1.);
	//master->addDevice("g1", g1);



	//create patches and patch them into master circuit
	for (int channel = 0; channel < CHANNELS; channel++) {
		inputs[channel] = new Patch;
		outputs[channel] = new Patch;
		//g0.addInput(inputs[channel]);
		//g0.addOutput(outputs[channel]);

		master->addInput(inputs[channel]);
		master->addOutput(outputs[channel]);
	}

	
	SineWaveGenerator* wave = new SineWaveGenerator;
	wave->setAmplitude(1.0);
	wave->setFrequency(0.5);
	wave->setPhase(0.);
	delete wave;
	//master->addDevice("wave", wave);

	Effect *dist = new Gain;
	dist->setBypass(false);
	dist->setParameter("level", 0.0000000000001);
	master->addDevice("dist", dist);

	//simple bypass circuit
	master->patch("in:0", "dist");
	//master->patch("in:1", "g1");
	master->patch("dist", "out:0");
	//master->patch("g1", "out:1");
	//master->patch("wave", "dist:level");

	master->optimize();

	//set up my data to pass to RT Audio call back
	MyData data;
	data.channels = CHANNELS;
	data.device = master;
	data.frameRate = 48000;
	data.frameSize = 8; //bytes
	data.keepGoing = true;
	data.input = inputs;
	data.output = outputs;

	//RT Audio stream parameters
	RtAudio::StreamParameters inputParams;
	inputParams.deviceId = 2; 
	inputParams.nChannels = CHANNELS;

	RtAudio::StreamParameters outputParams;
	outputParams.deviceId = 0;
	outputParams.nChannels = CHANNELS;

	//RT Audio Stream Options
	RtAudio::StreamOptions options;
	options.flags = RTAUDIO_MINIMIZE_LATENCY | RTAUDIO_HOG_DEVICE | RTAUDIO_SCHEDULE_REALTIME;
	options.numberOfBuffers = 4;
	options.streamName = "Patch-ware";
	options.priority = 0;

	//open interface
	RtAudio audioInterface;

	if (audioInterface.getDeviceCount() < 1) {
		std::cerr << "No audio devices found!\n";
		std::cin.get();
		goto CLEANUP;
	}

	std::cout << "Audio devices detected: " << audioInterface.getDeviceCount() << '\n';
	for (int device = 0; device < audioInterface.getDeviceCount(); device++) {
		std::cout << " Device[" << device << "]: " << audioInterface.getDeviceInfo(device).name << '\n';
	}
	

	//activate warning showing
	audioInterface.showWarnings();

	try {

		//open stream
		audioInterface.openStream(&outputParams, &inputParams, RTAUDIO_FLOAT64, data.frameRate, &bufferFrames, &call_back, &data, &options);

		//start stream
		audioInterface.startStream();
	
		//pause program
		std::cout << "Press [enter] to stop program: ";
		std::cin.get();

		//close stream
		audioInterface.closeStream();
	}
	catch (RtAudioError &e) {
		std::cerr << "RT Audio Error: " << e.what() << '\n';
		std::cin.get();
		goto CLEANUP;
	}

	std::cout << "Thank you for using Patch-Ware\nHave a nice day!";
	std::cin.get();
	//*****************************************

CLEANUP:
	for (int channel = 0; channel < CHANNELS; channel++) {
		delete inputs[channel];
		delete outputs[channel];
	}
	delete[] inputs;
	delete[] outputs;
	//delete master;
}

void timeTest() {

	Circuit circuit;

	Patch** in = new Patch*[2];
	in[0] = new Patch;
	in[1] = new Patch;

	Patch** out = new Patch*[2];
	out[0] = new Patch;
	out[1] = new Patch;

	circuit.addInput(in[0]);
	//circuit.addInput(in[1]);
	circuit.addOutput(out[0]);
	//circuit.addOutput(out[1]);

	Distortion *dist = new Distortion;
	dist->setBypass(false);
	circuit.addDevice("distortion", dist);

	SineWaveGenerator* sin = new SineWaveGenerator;
	sin->setFrequency(0.5);
	sin->setAmplitude(1.0);
	circuit.addDevice("sine", sin);

	circuit.patch("in:0", "distortion");
	circuit.patch("distortion", "out:0");
	circuit.patch("sin", "distortion:threshold");

	const int BUFF_SIZE = 48000;
	double* input = new double[BUFF_SIZE];
	double* output = new double[BUFF_SIZE];

	for (double i = 0.; i < BUFF_SIZE; i++) {
		input[(int)i] = i;
	}

	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < BUFF_SIZE; i++) {

		try {
			if (!in[0]->pushSignal(input[i])) {
				throw new std::exception("push[0] failure");
			}
			/*
			if (!in[1]->pushSignal(input[i])) {
				throw new std::exception("push[1] failure");
			}
			*/

			if (!circuit.process()) {
				throw new std::exception("process failure");
			}
			circuit.incrementTime(1. / 48000.);


			if (!out[0]->requestSignal(output[i])) {
				throw new std::exception("request[0] failure");
			}
			/*
			if (!out[1]->requestSignal(output[i])) {
				throw new std::exception("request[1] failure");
			}
			*/
		}
		catch (std::exception e) {
			std::cerr << e.what();
			break;
		}
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time taken for " << BUFF_SIZE << "stereo samples: " << diff.count() << "!";

	std::cin.get();

	bool passed = true;
	for (double i = 0; i < BUFF_SIZE; i++) {
		if (output[(int)i] != i) {
			passed = false;
			std::cout << "index[" << (int)i << "] does not match!\n";
			break;
		}
	}
	if (passed) {
		std::cout << "Test Passed!\n";
	}
	else {
		std::cout << "Test Failed\n";
	}
	std::cin.get();


	delete[] input;
	delete[] output;
	delete in[0];
	delete in[1];
	delete out[0];
	delete out[1];
	delete[] in;
	delete[] out;
}

bool traverse(double* data, void* arg) {
	*data += 1.f;
	return true;
}

void listTest() {
	LinkedList<double> linked;
	std::vector<double> vect;
	std::map<size_t, double> map;

	const size_t SIZE = 48000;
	vect.reserve(SIZE);

	//initialize racers
	for (size_t i = 0; i < SIZE; i++) {
		double* dub = new double;
		*dub = (double)i;
		linked.push_back(dub);
		vect.push_back(*dub);
		map[i] = (double)i;
	}

	//test linked list traversal
	auto linked_start = std::chrono::system_clock::now();
	linked.apply(traverse, NULL);//<---- way fucking fast !!!! Woo Hoo !!!!
	auto linked_end = std::chrono::system_clock::now();

	//test vector iteration
	auto vect_start = std::chrono::system_clock::now();
	for (int it = 0; it < SIZE;) {
		vect[it++] += 1.;
	}
	auto vect_end = std::chrono::system_clock::now();

	//test map iteration
	auto map_start = std::chrono::system_clock::now();
	for (auto it = map.begin(); it != map.end(); it++) {
		it->second += 1.;
	}
	auto map_end = std::chrono::system_clock::now();

	std::chrono::duration<double> linked_diff = linked_end - linked_start;
	std::chrono::duration<double> vect_diff = vect_end - vect_start;
	std::chrono::duration<double> map_diff = map_end - map_start;

	std::cout << "Linked Time: " << linked_diff.count() << "\nVector Time: " << vect_diff.count() << "\nMap Time: " << map_diff.count() << '\n';
	std::cin.get();

	linked.clear(true);
}

int main(){
    
	//listTest();

	//timeTest();

	rtAudioTest();

	//envelopeTest();

	//circuitTest();

    //sineTest();
    
    //patchDriverCopy();
    
    //println("Testing Waves");
    //waveTester();
    
    //println("Testing Filter");
    //filterTester();
    
    //println("Testing Compressor");
    //compressorTester();
    
    //println("Testing Distortion");
    //distortionTester();
    
    //println("Testing Delay");
    //delayTester();
    
    //println("Testing Gain");
    //gainTester();
    
    //println("Testing Biquad Filter");
    //biquadTester();
    
    //playSound();
    
    return 0;
}