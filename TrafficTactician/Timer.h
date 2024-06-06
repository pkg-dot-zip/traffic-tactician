#pragma once
#include <functional>

class Timer
{
private:
	std::function<void(int)> callback;
	bool isTimerOn = false;
	float timeElapsed = 0.0f;
	float rolloverTime = 5.0f;
	int rolloverCount = 0;
public:
	Timer(float rolloverTime);
	~Timer();

	void setCallback(std::function<void(int)> cb);
	void toggleTimer(bool x);
	void update(float deltaTime);
	float getTime();
};
