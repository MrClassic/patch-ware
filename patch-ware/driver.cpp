/* *************************************************************************
 * File:    driver.cpp
 * Author:  Nate Gallegos
 * Log
 *      5/4/17
 *      File Created
 ************************************************************************* */


#ifndef DRIVER_CPP
#define DRIVER_CPP

//#include <sys/ioctl.h>
//#include <unistd.h>
//#include <linux/kd.h>
#include <fstream>
#include <string>
#include <exception>
//#include <fftw3.h>
#include <chrono>
#include <iostream>
#include <filesystem>

#include "RtAudio.h"

//circuit
#include "ProcessorCluster.h"
#include "Circuit.h"

//effect includes
#include "SignalProcessor.h"
#include "Effect.h"
#include "Compressor.h"
#include "Distortion.h"
#include "InvertDistortion.h"
#include "Delay.h"
#include "BiquadFilter.h"
#include "Gain.h"
#include "Gate.h"
#include "FIRFilter.h"
#include "IIRFilter.h"

//wave generator includes
#include "WaveProcessor.h"
#include "SineWaveGenerator.h"
#include "SquareWaveGenerator.h"
#include "TriangleWaveGenerator.h"
#include "SawtoothWaveGenerator.h"
#include "QuadraticWaveGenerator.h"
#include "ZeroWaveGenerator.h"
#include "SignalSpy.h"
#include "PulseGenerator.h"

//envelope includes
#include "Envelope.h"
#include "LinearEnvelope.h"

#include "Parser.h"

#include "Biquad.h"

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

//Globals

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
	*filter->paramAddr(Filter::BYPASS) = 0.;
    //filter->setBypass(false); deprecated
    double push = 0.01;
    for(int i = 0; i < 15; i++){
        push += 0.01 * (double)i;
        showPush(*filter, push);
    }
    
    delete filter;
    filter = new IIRFilter;
    filter->setOrder(3);
	/*
    filter->setCoefficient(0, -0.1);
    filter->setCoefficient(1, -0.2);
    filter->setCoefficient(2, -0.3);
    filter->setCoefficient(3, -0.4);
	*/
	*filter->paramAddr(Filter::COEFFICIENTS + 0) = -0.1;
	*filter->paramAddr(Filter::COEFFICIENTS + 1) = -0.2;
	*filter->paramAddr(Filter::COEFFICIENTS + 2) = -0.3;
	*filter->paramAddr(Filter::COEFFICIENTS + 3) = -0.4;

	*filter->paramAddr(Filter::BYPASS) = 0.;
	//filter->setBypass(false); deprecated
    push = 0.01;
    for(int i = 0; i < 15; i++){
        push += 0.01 * (double)i;
        showPush(*filter, push);
    }
    
    delete filter;
    
}
/*
void fftwTest() {

	const int SIZE = 32;
	const int REAL = 0;
	const int IMAG = 1;
	fftw_complex input[SIZE];
	fftw_complex output[SIZE];

	double result;
	SineWaveGenerator* sine = new SineWaveGenerator;
	sine->addToOutputs(&result);
	sine->frameRate = 32;
	*(sine->paramAddr(sine->FREQUENCY)) = 1.0;
	*(sine->paramAddr(sine->AMPLITUDE)) = 1.0;
	*(sine->paramAddr(sine->PHASE)) = 0.0;

	for (int i = 0; i < SIZE; i++) {
		sine->process();
		input[i][REAL] = result;
		input[i][IMAG] = 0.0;
	}

	fftw_plan plan = fftw_plan_dft_1d(SIZE, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(plan);

	fftw_destroy_plan(plan);

	fftw_cleanup();

	for (int i = 0; i < SIZE; i++) {
		output[i][REAL] /= SIZE;
		output[i][IMAG] /= SIZE;
	}

	print("input:\n");
	for (int i = 0; i < SIZE; i++) {
		print("   ");
		print(input[i][REAL]);
		print(" ");
		print(input[i][IMAG]);
		print("i\n");

	}

	print("\n\noutput:\n");
	for (int i = 0; i < SIZE; i++) {
		print("   ");
		print(output[i][REAL]);
		print(" ");
		print(output[i][IMAG]);
		print("i\n");
	}

	std::cin.get();
}
*/
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
/*
void patchDriverCopy(){
    
    //master lists
    LinkedList<Patch> patchMaster;
    LinkedList<Effect> effectMaster;
    LinkedList<WaveGenerator> genMaster;
    
    double phase = 0.;
    double amp = 1.;
    double freq = 1.;
    
    //wave generator for parameter controls
    SineWaveGenerator *param = new SineWaveGenerator;
    param->params;
    param->setAmplitude(2);
    param->setFrequency(1.7);
    
    //wave generator for signal boosting
    WaveGenerator *boost = new WaveGenerator(new ZeroWaveGenerator(4.00001));
    
    //sine wave generator for signal
    WaveGenerator *signal = new WaveGenerator(new SquareWaveGenerator);
    signal->setAmplitude(1.);
    signal->setFrequency(8.0);
    signal->setPhase(0.);
    
    //add to master lists
    genMaster.push_back(param);
    genMaster.push_back(boost);
    genMaster.push_back(signal);
    
    //Gain effect
    Effect* gain = new Effect(new Gain());
	*gain->getProc()->paramAddr(Gain::BYPASS) = 0.;
	*gain->getProc()->paramAddr(Gain::LEVEL) = 1.0;
	//gain->setParameter("level", 1.0); deprecated
    //gain->setLevel(1.0);
    //gain->setBypass(false); deprectaed
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
    Effect* gain = new Effect(new Gain());
	*gain->getProc()->paramAddr(Gain::BYPASS) = 0.;
	*gain->getProc()->paramAddr(Gain::LEVEL) = 1.;
	gain->getProc()->setInputType(SUM);

    //gain->setParameter("level", 1.0);
    //gain->setBypass(false);
    //gain->setInputType(SUM);
    
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
	Effect *gain = new Effect(new Gain);
	*gain->getProc()->paramAddr(Gain::BYPASS) = 0.;
	*gain->getProc()->paramAddr(Gain::LEVEL) = 1.;
	gain->getProc()->setInputType(SUM);

	//gain->setParameter("level", 1.);
	//gain->setInputType(input_type::SUM);
	
	circuit.addDevice("gain", gain);
	

	//FIR filter
	IIRFilter* fir = new IIRFilter;
	//fir->setBypass(true);

	fir->setOrder(10);
	//set up coefficients
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 0) = 1.;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 1) = -0.08;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 2) = -0.04;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 3) = -0.03;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 4) = 0.035;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 5) = -0.025;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 6) = -0.625;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 7) = 0.125;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 8) = 0.025;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 9) = -0.15;
	*fir->paramAddr(IIRFilter::COEFFICIENTS + 10) = -0.1;
	//fir->setCoefficient(0, 1.);
	//fir->setCoefficient(1, -0.08);
	//fir->setCoefficient(2, -0.04);
	//fir->setCoefficient(3, -0.03);
	//fir->setCoefficient(4, 0.035);
	//fir->setCoefficient(5, -0.025);
	//fir->setCoefficient(6, -0.625);
	//fir->setCoefficient(7, 0.125);
	//fir->setCoefficient(8, 0.025);
	//fir->setCoefficient(9, -.15);
	//fir->setCoefficient(10, -0.1);
	
	circuit.addDevice("fir", new Effect(fir));
	
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
	*gain->paramAddr(Gain::BYPASS) = 0.;
	*gain->paramAddr(Gain::LEVEL) = 1.;

	//gain->setParameter("level", 1.0);
	
	circuit.addDevice("gain", new Effect(gain));

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
*/
struct ProcessingBlock {
	double* inputs, **outputs, *params;
	ProcessingBlock** next;
	int _inputs, _outputs, _params, _nexts;

	virtual void proc() = 0;
};

struct MyData{
	unsigned int channels;
	unsigned int frameRate;
	unsigned int frameSize;
	double** input;
	double* output;
	ProcessorCluster* pc;
	bool keepGoing;
};

bool proc(Processor* data, void*) {
	data->process();
	return true;
}

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
				*myData->input[channel] = iBuf[index++];
				/*
				if (!myData->input[channel]->pushSignal(iBuf[index++])) {
					std::string message = "Push signal failed. channel: " + std::to_string(channel) + '\n';
					throw std::exception(message.c_str());
				}
				*/
				//if(sample % 100 == 0)
					//std::cout << "input[" << index - 1 << "] = " << iBuf[index - 1] << '\n';
			}

			//process the circuit
			//if (!myData->device->process())
				//throw std::exception("Patch Device could not process");

			//for (int block = 0; block < myData->blockCount; block++) {
			//	myData->blocks[block]->proc();
			//}

			//process for all processors in group
			myData->pc->process();

			//increment time
			//myData->device->incrementTime(1. / (double)myData->frameRate);

			//load output(s) from patches
			for (int channel = 0; channel < myData->channels; channel++) {
				oBuf[outdex++] = myData->output[channel];
				//if (!myData->output[channel]->requestSignal(oBuf[outdex++])) {
				//	std::string message = "Request signal failed. channel: " + std::to_string(channel) + '\n';
				//	throw std::exception(message.c_str());
				//}
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


struct gain : ProcessingBlock {
	const int GAIN = 0;
	gain() {
		_params = 1;
		params = new double[_params];
		params[GAIN] = 1.0;
		_inputs = 1;
		inputs = new double[_inputs];
		_outputs = 1;
		outputs = new double*[_outputs];
		_nexts = _outputs;
		next = new ProcessingBlock*[_nexts];
	}
	~gain() {
		delete params;
		delete inputs;
		delete outputs;
		delete next;
	}
	void proc() {
		double signal = inputs[0];
		for (int i = 1; i < _inputs; i++) {
			signal += inputs[i];
		}
		signal /= _inputs;
		signal *= params[GAIN];
		for (int o = 0; o < _outputs; o++) {
			*outputs[o] = signal;
		}
		for (int n = 0; n < _nexts; n++) {
			next[n]->proc();
		}
	}
};
struct adder : ProcessingBlock {
	const int LEVEL = 0;
	adder() {
		_params = 1;
		params = new double[_params];
		params[LEVEL] = 1.0;
		_inputs = 1;
		inputs = new double[_inputs];
		_outputs = 1;
		outputs = new double*[_outputs];
		_nexts = _outputs;
		next = new ProcessingBlock*[_nexts];
	}
	~adder() {
		delete params;
		delete inputs;
		delete outputs;
		delete next;
	}
	void proc() {
		double signal = inputs[0];
		if (inputs != NULL) {
			for (int i = 1; i < _inputs; i++) {
				signal += inputs[i];
			}
		}
		signal /= _inputs;
		signal += params[LEVEL];
		if (outputs != NULL) {
			for (int o = 0; o < _outputs; o++) {
				*outputs[o] = signal;
			}
		}
		if (next != NULL) {
			for (int n = 0; n < _nexts; n++) {
				next[n]->proc();
			}
		}
	}
};
struct dist : ProcessingBlock {
	const int THRESH = 0;
	int inputCounter = 0;
	int procCount = 0;
	dist() {
		_params = 1;
		params = new double[_params];
		params[THRESH] = 1.0;
		_inputs = 1;
		inputs = new double[_inputs];
		_outputs = 1;
		outputs = new double*[_outputs];
		_nexts = _outputs;
		next = new ProcessingBlock*[_nexts];
	}
	~dist() {
		delete params;
		delete inputs;
		delete outputs;
		delete next;
	}
	void proc() {
		++inputCounter;
		if (inputCounter == 2) {
			++procCount;
			inputCounter = 0;
			double signal = inputs[0];
			for (int i = 1; i < _inputs; i++) {
				signal += inputs[i];
			}
			signal /= _inputs;

			if (signal > params[THRESH]) {
				signal = params[THRESH];
			}
			else if (signal < (params[THRESH] * -1)) {
				signal = params[THRESH] * -1;
			}

			for (int o = 0; o < _outputs; o++) {
				*outputs[o] = signal;
			}
			/*
			for (int n = 0; n < _nexts; n++) {
				next[n]->proc();
			}
			*/
		}
	}
};
struct sine : ProcessingBlock {
	const int AMP = 0;
	const int FREQ = 1;
	const int PHASE = 2;
	const double F_RATE = 1. / 48000.;
	double time;
	

	sine() {
		time = 0;
		_params = 3;
		params = new double[_params];
		params[AMP] = 1.0;
		params[FREQ] = 1.0;
		params[PHASE] = 0.0;
		_inputs = 0;
		inputs = NULL;
		_outputs = 1;
		outputs = new double*[_outputs];
		_nexts = _outputs;
		next = new ProcessingBlock*[_nexts];
	}
	~sine() {
		delete params;
		//delete inputs;
		delete outputs;
		delete next;
	}
	void proc() {
		/*
		double signal = inputs[0];
		for (int i = 1; i < _inputs; i++) {
			signal += inputs[i];
		}
		signal /= _inputs;
		*/
		double signal = params[AMP] * sineD((time * params[FREQ] + params[PHASE])* 360.);
		time += F_RATE;
		while (time > 1. / params[FREQ]) {
			time -= (1. / params[FREQ]);
		}
		for (int o = 0; o < _outputs; o++) {
			*outputs[o] = signal;
		}
		for (int n = 0; n < _nexts; n++) {
			next[n]->proc();
		}
	}
};


void rtAudioTest() {

	//number of channels
	const unsigned int CHANNELS = 1;
	unsigned int bufferFrames = 1;

	double* _inputs[CHANNELS];
	double _outputs[CHANNELS];

	LinkedList<Processor> proc_master;
	
	Circuit* generated = fileToCircuit("C:\\GitHub\\patch-ware\\patch-ware\\CheapReverb.xml");
	if (generated == NULL) {
		error("there was a problem somewhere...");
		std::cin.get();
		return;
	}
	generated->optimize();
	ProcessorCluster* pc = generated->exportAsProcessor();
	//ProcessorCluster* pc = master->exportAsPrcoessor();
	for (int channel = 0; channel < CHANNELS; ++channel) {
		_inputs[channel] = pc->inputAddr(channel);
		pc->setOutputChannel(channel, &_outputs[channel]);
	}

	//set up my data to pass to RT Audio call back
	MyData data;
	data.channels = CHANNELS;
	data.frameRate = 48000;
	data.frameSize = 8; //bytes
	data.pc = pc;
	data.keepGoing = true;
	data.input = _inputs;
	data.output = _outputs;

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
	
	proc_master.clear(true);
	//delete[] inputs;
	//delete[] outputs;
	//delete master;
}


void timeTest() {

	const unsigned int CHANNELS = 1;
	unsigned int bufferFrames = 512;

	double* _inputs[CHANNELS];
	double _outputs[CHANNELS];

	LinkedList<Processor> proc_master;

	//input patches for interfacing with RT Audio
	Patch* inputs[CHANNELS];// = new Patch*;

	//output patches for interfacing with RT Audio
	Patch* outputs[CHANNELS];// = new Patch*;

	//master circuit
	Circuit* master = new Circuit;

	//create patches and patch them into master circuit
	for (int channel = 0; channel < CHANNELS; channel++) {
		inputs[channel] = new Patch;
		outputs[channel] = new Patch;
		//g0.addInput(inputs[channel]);
		//g0.addOutput(outputs[channel]);

		master->addInput(inputs[channel]);
		master->addOutput(outputs[channel]);
	}



	Circuit* generated = fileToCircuit("C:\\GitHub\\patch-ware\\patch-ware\\CheapReverb.xml");
	if (generated == NULL) {
		error("there was a problem somewhere...");
		std::cin.get();
		return;
	}
	generated->optimize();
	//ProcessorCluster* pc = master->exportAsPrcoessor();
	ProcessorCluster* pc = generated->exportAsProcessor();

	for (int channel = 0; channel < CHANNELS; ++channel) {
		_inputs[channel] = pc->inputAddr(channel);
		pc->setOutputChannel(channel, &_outputs[channel]);
	}

	const int BUFF_SIZE = 48000;
	double input[BUFF_SIZE];
	double output[BUFF_SIZE];

	for (double i = 0.; i < BUFF_SIZE; i++) {
		input[(int)i] = i / BUFF_SIZE;
	}
	_inputs[0] = pc->inputAddr(0);
	*pc->outputAddr(0) = &_outputs[0];
	
	auto start = std::chrono::system_clock::now();

	for (int i = 0; i < BUFF_SIZE; i++) {

		try {
			/*
			if (!in[0]->pushSignal(input[i])) {
				throw new std::exception("push[0] failure");
			}
			/*
			/*
			if (!in[1]->pushSignal(input[i])) {
				throw new std::exception("push[1] failure");
			}
			*/

			*_inputs[0] = input[i];
			/*
			if (!circuit.process()) {
				throw new std::exception("process failure");
			}
			circuit.incrementTime(1. / 48000.);
			*/
			//_sine.proc();
			//_gainIn.proc();
			pc->process();
			/*
			if (!out[0]->requestSignal(output[i])) {
				throw new std::exception("request[0] failure");
			}
			*/
			/*
			if (!out[1]->requestSignal(output[i])) {
				throw new std::exception("request[1] failure");
			}
			*/
			output[i] = _outputs[0] ;

		}
		catch (std::exception e) {
			std::cerr << e.what();
			break;
		}
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "Time taken for " << BUFF_SIZE << " stereo samples: " << diff.count() << "!";

	std::cin.get();

	bool passed = true;
	for (double i = 0; i < BUFF_SIZE; i++) {
		if(BUFF_SIZE < 100)
			std::cout << "input: " << input[(int)i] << ",  output[" << (int)i << "]: " << output[(int)i] << '\n';
		if (output[(int)i] != i) {
			passed = false;
			//std::cout << "index[" << (int)i << "] does not match!\n";
			//break;
		}
	}
	if (passed) {
		std::cout << "Test Passed!\n";
	}
	else {
		std::cout << "Test Failed\n";
	}
	std::cin.get();

	proc_master.clear(true);
	if (pc != NULL) {
		//pc->getOrderList().clear(true);
		delete pc;
	}
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

/*
void fftwTest() {

	const double frameRate = 44000.;

	SineWaveGenerator gen;
	gen.setFrequency(440.);
	gen.setAmplitude(1.);
	gen.setPhase(0.);

	Patch outPatch;

	gen.addOutput(&outPatch);

	const int size = 400;
	fftw_complex in[size], out[size];
	fftw_plan plan;

	plan = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD, FFTW_MEASURE);
	

	double output;
	const int MAX_IT = 10;

	for (int iteration = 0; iteration < MAX_IT; iteration++) {
		//populate input array
		for (int i = 0; i < size; i++) {
			gen.process();
			if (outPatch.requestSignal(output)) {
				in[i][0] = output;
				in[i][1] = 0.;
			}
			else {
				std::cout << "error requesting signal from patch...";
				std::cin.get();
				goto CLEANUP;
			}
		}

		std::cout << "iteration " << iteration << '\n';

		for (int i = 0; i < size; i++) {
			std::cout << "out[" << i << "]: real: " << out[i][0] << " imag: " << out[i][1] << '\n';
		}
	}
	CLEANUP:
	fftw_destroy_plan(plan);
	std::cin.get();
}
*/

void fileReadTest() {

	std::string filename = "C:\\GitHub\\patch-ware\\patch-ware\\CheapReverb.xml";
	Circuit* circuit = fileToCircuit(filename);

	if (circuit) {
		std::cout << "success...?\n";
	}
	else {
		std::cout << "failed!!!!\n";
	}
	std::cin.get();
}

void biquadTest() {
	Biquad b;
	BiquadPeak peak;
	
	b.setType(bq_type_peak);
	b.setFc(500. / 48000.);
	b.setPeakGain(6.);
	b.setQ(0.7);

	*peak.paramAddr(BiquadPeak::Q) = 0.7;
	*peak.paramAddr(BiquadPeak::FC) = 500;
	*peak.paramAddr(BiquadPeak::PEAK) = 6.;
	double* input = peak.addInput();
	double output;
	peak.addToOutputs(&output);

	const int SIZE = 100;

	double in[SIZE];
	for (int i = 0; i < SIZE; ++i) {
		in[i] = (double)i / (double)SIZE;
	}

	double outs[2][SIZE];

	for (int i = 0; i < SIZE; i++) {
		outs[1][i] = peak.processSignal(in[i]);
		outs[0][i] = b.process(in[i]);
		
	}

	for (int i = 0; i < SIZE; i++) {
		std::cout << "Nigels's biquad[" << i << "]: " << outs[0][i] << ", my biquad[" << i << "]: " << outs[1][i] << '\n';
	}

	std::cin.get();
}



int main(){
    
	//biquadTest();

	//fileReadTest();

	//listTest();

	timeTest();

	//fftwTest();

	//rtAudioTest();

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

#endif

//EOF