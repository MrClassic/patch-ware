/*
	File: Timer.h
	Purpose: To handle the logic associated with tracking and updating a time variable.
		Useful for managing different time invariant Devices with a single interface.

	Log:
		8/12/18 - N8
			File Created

*/
#ifndef TIMER_H
#define TIMER_H

class Timer {
public:

	Timer();

	Timer(const Timer &rhs);

	virtual ~Timer() {};

	virtual void incrementTime(const double time) = 0;

	double getTime() const;

protected:

	//my current time
	double currentTime;
};

#endif // !TIMER_H
