

#ifndef ANON_EFFECT_H
#define ANON_EFFECT_H

#include "Effect.h"

class AnonymousEffect : public Effect {

	//sets the anonymous processing function to a bypass function.
	AnonymousEffect();

	//initializes the anonymous processing function
	AnonymousEffect(double(*func)(Effect*, const double));

	//sets the anonymous processing function
	void setFunction(double(*func)(Effect*, const double));

	//concrete implementation of process(),
	//attempts to call the anonymous function pointer as
	//the following line:
	//
	//	output( function( this, input() ) );
	//
	//so that the calling effect and the input are passed to the
	//anonymous processing method, and the function's return
	//is sent to the output channel(s)
	bool process();

public:


private:

	//anonymous function pointer
	//Effect* will be the calling AnonymousEffect object (this)
	//double will be the signal from the input device
	double (*function)(Effect*, const double);

	
};

//sample anonymous processing function.
//Effect* effect = the calling Effect:
//		use this pointer to access parameters for processing
//		effect->getParameter("tag", paramPtr)
//double signal = the signal retrieved from the input channel(s)
double bypassFunction(Effect* effect, const double signal) {
	return signal;
}

#endif // !ANON_EFFECT_h
