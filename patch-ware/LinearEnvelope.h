

#ifndef LINEAR_ENVELOPE_H
#define LINEAR_ENVELOPE_H

#include "Envelope.h"


class LinearEnvelope : public Envelope {

public:
	//sets the base = 0 and threshold = 1
	LinearEnvelope();
protected:
	//linear decay envelope
	double envelope(double const signal);

};
#endif

//EOF