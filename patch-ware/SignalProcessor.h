

#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

//lol, homie = friend
#define homie friend

#include "Processor.h"

class SignalProcessor : public Processor{

public:
	SignalProcessor();

	~SignalProcessor();

	void process();

	homie class Effect;
	homie class Envelope;
	homie class Circuit;

protected:

	virtual double processSignal(const double &) = 0;

};

#endif
