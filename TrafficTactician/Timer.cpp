#include "Timer.h"

Timer::Timer(float rolloverTime) : rolloverTime(rolloverTime)
{
}

Timer::~Timer()
{
}

float Timer::getTime()
{
	return timeElapsed;
}

void Timer::setCallback(std::function<void(int)> cb) {
	callback = cb;
}

void Timer::toggleTimer(bool x) {
	isTimerOn = x;
}

void Timer::update(float deltaTime) {
	if (!isTimerOn)
		return;
	timeElapsed += deltaTime;

	if (timeElapsed >= rolloverTime) {
		rolloverCount++;
		timeElapsed -= rolloverTime; // or timeElapsed = 0.0f; if you want to completely reset
		if (callback) {
			callback(rolloverCount);
		}
	}
}