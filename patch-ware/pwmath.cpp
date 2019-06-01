/* **********************************************************************
 * File:   pwmath.cpp
 * Author: Nate Gallegos
 *
 * Purpose: To provide a light weight include to handle basic math functions.
 *			As more functions are needed, they should be added to this file.
 * 
 * Log
 *      4/30/17
 *      File Created
 *      added pw_abs() for doing absolute values
 *      TO DO:
 *      add more math function as the project continues
 ********************************************************************* */

#include "pwmath.h"

//Basic absolute value function
//do I really need to explain this...?
double pw_abs(double input) {
	if (input >= 0) {
		return input;
	}
	else {
		return input * -1.;
	}
}