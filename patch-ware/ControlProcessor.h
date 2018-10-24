

#ifndef CONTROL_PROCESSOR_H
#define CONTROL_PROCESSOR_H



#include "Processor.h"

class ControlProcessor : public Processor {

public:
	ControlProcessor() : Processor() { /* do nothing */ };

	virtual ~ControlProcessor() { /* do nothing */ };

	virtual void process() = 0;

protected:


};

#endif