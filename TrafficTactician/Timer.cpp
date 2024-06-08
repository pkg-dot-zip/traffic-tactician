#include "Timer.h"

#include <ostream>

#include "easylogging++.h"

Timer::Timer(const std::function<void()>& callback, float rolloverTime)
{
	this->rolloverTime = rolloverTime;
	this->callback = callback;
}

float Timer::getTime() const
{
	return timeElapsed;
}

float Timer::getTotalElapsedTime() const
{
	return timeElapsed + rolloverCount * rolloverTime;
}

float Timer::getTimeRemaining() const
{
	return rolloverTime - timeElapsed;
}

void Timer::setCallback(const std::function<void()>& cb) {
	callback = cb;
}

void Timer::toggleTimer(bool x) {
	isTimerOn = x;
}

void Timer::update(float deltaTime) {
	if (!isTimerOn) return;
	timeElapsed += deltaTime;

	if (timeElapsed >= rolloverTime) {
		rolloverCount++;
		timeElapsed -= rolloverTime;

		// TODO: Check if callback is nullptr.
		if (callback) {
			LOG(INFO) << "Timer passed. Running callback." << std::endl;
			callback();
		}
	}
}
