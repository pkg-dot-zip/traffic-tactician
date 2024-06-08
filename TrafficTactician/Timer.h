#pragma once
#include <functional>

class Timer
{
private:
	std::function<void()> callback;
	bool isTimerOn = false;
	float timeElapsed = 0.0f;
	int rolloverCount = 0;
public:
	float rolloverTime = 0.0f;

	Timer(float rolloverTime = 5.0f);
	~Timer();

	void setCallback(std::function<void()> cb);
	void toggleTimer(bool x);
	void update(float deltaTime);
	float getTime();
	float getTotalElapsedTime();
	float getTimeRemaining();
};
