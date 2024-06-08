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

float Timer::getTotalElapsedTime()
{
	return timeElapsed + rolloverCount * rolloverTime;
}

float Timer::getTimeRemaining()
{
	return rolloverTime - timeElapsed;
}

void Timer::setCallback(std::function<void()> cb) {
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
		timeElapsed -= rolloverTime;
		if (callback) {
			callback();
		}
	}
}