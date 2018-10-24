
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>

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
#include "BiquadBandpass.h"
#include "BiquadPeak.h"
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
//#include "driver.cpp"

#include "Error.h"


void eatUntil(std::ifstream &file, char find);

void eatThese(std::ifstream &file, char find);

void fillUntil(std::ifstream &file, char find, std::string &ret);

PatchDevice* stringToDevice(const std::string &str);

Circuit* fileToCircuit(const std::string &filename);


#endif

//EOF