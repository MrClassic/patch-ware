
#include "AnonymousEffect.h"

AnonymousEffect::AnonymousEffect() {
	function = &bypassFunction;
}

AnonymousEffect::AnonymousEffect(double(*func)(Effect*, const double)) {
	function = func;
}

//sets the anonymous processing function
void AnonymousEffect::setFunction(double(*func)(Effect*, const double)) {
	function = func;
}

//attempts to process the function.
bool AnonymousEffect::process() {
	if (!*this || !parametersReady()) {
		return false;
	}
	if (function == NULL || params["bypass"]) {

		//input >>> output
		output(input());
	}
	else {
		//send the input signal to the function for processing,
		//then out to the output channel(s)

		//input >>> function >>> output
		output(function(this, input()));
	}
	return true;
}

