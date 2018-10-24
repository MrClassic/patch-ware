

#ifndef LINEAR_ENVELOPE_H
#define LINEAR_ENVELOPE_H

#include "EnvelopeProcessor.h"


class LinearEnvelope : public EnvelopeProcessor {

public:
	//sets the base = 0 and threshold = 1
	LinearEnvelope();
protected:
	//linear decay envelope
	double envelope();

};
#endif

//EOF