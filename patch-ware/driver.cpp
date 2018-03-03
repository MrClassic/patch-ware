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

void testFramework(){
    
    //master lists
    LinkedList<Patch> patchMaster;
    LinkedList<Effect> effectMaster;
    LinkedList<WaveGenerator> genMaster;
    
    //sine wave geenerator for parameter controls
    SineWaveGenerator *param = new SineWaveGenerator();
    param->setPhase(0.);
    param->setAmplitude(1.0);
    param->setFrequency(0.5);
    
    //sine wave generator for signal
    SineWaveGenerator *signal = new SineWaveGenerator();
    signal->setAmplitude(1.);
    signal->setFrequency(1.);
    signal->setPhase(0.);
    
    //add to master lists
    genMaster.push_back(param);
    genMaster.push_back(signal);
    
    //Gain effect
    Gain* gain = new Gain();
    gain->setLevel(1.0);
    gain->setBypass(false);
    effectMaster.push_back(gain);
    
    //patches
    Patch *gain_p = new Patch();
    Patch *gain_in = new Patch();
    Patch *gain_out = new Patch();
    
    //patch param generator to gain's level
    gain->getLevel().addInput(gain_p);
    param->addOutput(gain_p);
    
    //patch signal generator to gain's input
    signal->addOutput(gain_in);
    gain->addInput(gain_in);
    
    //patch gain's output to gain_out patch
    gain->addOutput(gain_out);
    
    
    //loop conditions
    const double deltaT = 0.1;
    const double seconds = 2;
    
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
                if(!fxPop->process()){
                    fx.push_back(fxPop);
                    fxPops++;
                }
                else{
                    fxPops = 0;
                }
            }
        }
        
        //print signal to cout
        double signalOut = 0.0;
        gain_out->requestSignal(signalOut);
        print(signalOut);
        println(",");
    }
    
    //clean up memory
    genMaster.clear(true);
    effectMaster.clear(true);
    patchMaster.clear(true);
}



int main(){
    
    testFramework();
    
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