/*#include "Effect.h"
#include "BiquadFilter.h"
#include "Circuit.h"
#include "Compressor.h"
#include "Delay.h"
#include "Distortion.h"
#include "Gain.h"
#include "Gate.h"
#include "InvertDistortion.h"
#include "SignalSpy.h"


#ifndef INCLUDEFX_H
#define INCLUDEFX_H


enum EFFECTS {
	BIQUAD_FILTER = 0,
	CIRCUIT,
	COMPRESSOR,
	DELAY,
	DISTORTION,
	FIR_FILTER,
	GAIN,
	GATE,
	IIR_FILTER,
	INVERT_DISTORTION,
	SIGNAL_SPY,

	//... insert new Effects as they are made

	NUM_OF_EFFECTS
};

EFFECTS getEffectType(OutputDevice* device) {

	if (dynamic_cast<BiquadFilter*>(device) != NULL) {
		return EFFECTS::BIQUAD_FILTER;
	}
	if (dynamic_cast<Circuit*>(device) != NULL) {
		return EFFECTS::CIRCUIT;
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
#endif
*/