#include "Timer.h"

#include <ostream>

#include "easylogging++.h"
#include "SoundHandler.h"

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

	// PlaySound logic.
	soundPassTime += deltaTime;
	if (soundPassTime >= 1.0F)
	{
		SoundHandler::getInstance().playSoundSnippet(alternateSound ? "sounds/tick_1.wav" : "sounds/tick_2.wav");
		soundPassTime -= 1.0F;
		alternateSound = !alternateSound;
	}

	// If timer passed.
	if (timeElapsed >= rolloverTime) {
		rolloverCount++;
		timeElapsed -= rolloverTime;

		if (callback != nullptr) {
			LOG(INFO) << "Timer passed. Running callback." << std::endl;
			callback();
		}
	}
}
