/*
	File:	Envelope.h
	Author: Nate Gallegos

	Abstract Method:
		
			double envelope(const double signal)

	Implementing Abstract Classes:
			
			PatchDevice
			Parameterizable
			Timer

	Parameters Defined:

			"threshold"	-	The minimum signal to trigger the start of the
							envelope function.
			"base" -		The signal to output while waiting for an input
							signal to trigger the envelope function.
			"duration" -	The duration (in seconds) for the envelope function.
	
	Purpose: Envelopes are patch devices that wait for their input signal
	to exceed a threshold. While they are waiting, they output a "base"
	signal. After the threshold has been exceeded, they start an output function
	over time. The output function is not defined in the Envelope class, but
	can be defined in a sub-class by implementing the:
			
			double envelope(const double signal);

	method. This abstract class handles the logic for waiting for the signal threshold
	to be broken, envelope function activation, and time incrementing. The currentTime
	variable inherited from Timer will start at 0 when the envelope is activated and will
	reset when the timer reaches or exceeds the "duration" parameter. The reset boolean
	flags whether the enveloping function can restart if the threshold is exceeded mid
	envelope or whether any threshold breaking signals will be ignored while the envelope
	is outputting its function.

	************************************************************************************

	History Log:
		
		8/20/18 - N8
			file created and documented
			Implemented base features

*/

//include guard
#ifndef ENVELOPE_H
#define ENVELOPE_H

//patchware includes
#include "Parameterizable.h"
#include "Timer.h"

//Envelope class
class Envelope : public InputDevice, public OutputDevice, public Parameterizable, public Timer{

//public shit
public:

	Envelope();

	Envelope(const Envelope &rhs);

	//Timer definition
	virtual void incrementTime(const double);

	//Patch device definition
	virtual bool process();


//protected shit
protected:
	
	//envelope function accepts the input signal,
	//may be used or not, thats up to the implementer
	virtual double envelope(const double signal) = 0;

	//boolean for whether the envelope is activated or not
	bool on;

	//boolean for whether the envelope resets if
	//triggered while active, or if it should
	//ignore threshold triggers while active
	bool reset;

};

#endif // !ENVELOPE_H

//EOF