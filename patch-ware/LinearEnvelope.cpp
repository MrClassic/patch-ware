

#include "LinearEnvelope.h"

LinearEnvelope::LinearEnvelope() {
	params["base"] = 0.;
	params["threshold"] = 1.;
}


double LinearEnvelope::envelope(const double signal) {
	//signal not used
	return params["threshold"] * (1. - (currentTime / params["duration"]));
}

//EOF