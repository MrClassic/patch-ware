

#include "LinearEnvelope.h"

LinearEnvelope::LinearEnvelope() : EnvelopeProcessor() {
	/* do nothing...?*/
}

double LinearEnvelope::envelope() {
	return params[THRESH] * (1. - (currentTime / params[DURATION]));
}

//EOF