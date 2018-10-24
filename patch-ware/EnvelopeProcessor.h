


#ifndef ENVELOPE_PROCESSOR_H
#define ENVELOPE_PROCESSOR_H

#include "SignalProcessor.h"

class EnvelopeProcessor : public SignalProcessor{

public:

	EnvelopeProcessor();

	EnvelopeProcessor(const EnvelopeProcessor&);

	~EnvelopeProcessor();


	enum parameters {
		THRESH,
		BASE,
		DURATION,
		NUM_PARAMS
	};

	friend class Envelope;

protected:

	double processSignal(const double &);

	virtual double envelope() = 0;

	//boolean for whether the envelope is activated or not
	bool on;

	//boolean for whether the envelope resets if
	//triggered while active, or if it should
	//ignore threshold triggers while active
	bool reset;



};

#endif