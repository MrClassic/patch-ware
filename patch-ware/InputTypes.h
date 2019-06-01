/* **********************************************************************
 * File:   InputTypes.h
 * Author: Nate Gallegos
 *
 * Purpose: Houses the input_type enum for InputDevices.
 *				I'm not sure why I abstracted this to it's own file.
				--surely I had a reason...
 *
 * Log
 *      6/1/19
 *		Log created
 ********************************************************************* */


#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

//input_type enum
//basic enum for storing an InputDevice's Input Type
//The input type defines that Input Device's algorithm
//for collecting and condensing multiple inputs into a 
//single signal.

enum input_type {
	AVERAGE = 0,
	SUM, 
	PRODUCT,
	MIN,
	MAX,
	NUM_OF_TYPES
};

#endif