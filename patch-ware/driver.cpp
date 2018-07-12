/* *************************************************************************
 * File:    driver.cpp
 * Author:  Nate Gallegos
 * Log
 *      5/4/17
 *      File Created
 ************************************************************************* */

#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/kd.h>
#include <fstream>


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


void playSound(){
    int freq[] = { /* C   D    E    F    G    A    B    C */
                    523, 587, 659, 698, 784, 880, 988, 1046 };
    int i;

    for (i=0; i<8; i++)
    {
            ioctl(STDOUT_FILENO, KIOCSOUND, 1193180/freq[i]);
            usleep(500000);
    }
    ioctl(STDOUT_FILENO, KIOCSOUND, 0); /*Stop silly sound*/
}
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
    WaveGenerator *signal = new SineWaveGenerator();
    signal->setAmplitude(1.);
    signal->setFrequency(8.0);
    signal->setPhase(0.);
    
    //add to master lists
    genMaster.push_back(param);
    genMaster.push_back(boost);
    genMaster.push_back(signal);
    
    //Gain effect
    Gain* gain = new Gain();
    gain->setLevel(1.0);
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
    patchMaster.push_back(patchDevices(paramSpy, &signal->getFrequency()));
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
                    genPop->pushDouble();
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
    gain->setLevel(1.0);
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
                    genPop->pushDouble();
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

int main(){
    
    //sineTest();
    
    patchDriver();
    
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