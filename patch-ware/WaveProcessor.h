

#ifndef WAVE_PROCESSOR_H
#define WAVE_PROCESSOR_H


#include "ControlProcessor.h"

class WaveProcessor : public ControlProcessor {

public:
	WaveProcessor();

	WaveProcessor(const WaveProcessor &);

	~WaveProcessor();

	double getPhaseOffset();

	virtual void updateWaveOffset() = 0;

	void process();

	enum parameters {
		FREQUENCY = 0,
		AMPLITUDE,
		PHASE,
		NUM_PARAMS
	};

	friend class WaveGenerator;
	friend class Circuit;

protected:

	double lastFreq, lastPhase, lastAmp, phaseCorrector;

	
	
	virtual double generate() = 0;

};

#endif