
#include "Timer.h"

Timer::Timer() {
	currentTime = 0.;
}

Timer::Timer(const Timer &rhs) {
	currentTime = rhs.currentTime;
}

double Timer::getTime() const {
	return currentTime;
}

