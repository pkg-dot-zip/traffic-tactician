#pragma once
#include <functional>

class Timer
{
public:
	float rolloverTime = 0.0f;

	Timer(const std::function<void()>& callback = nullptr, float rolloverTime = 5.0f);
	~Timer() = default;

	void setCallback(const std::function<void()>& cb);
	void toggleTimer(bool x);
	void update(float deltaTime);
	float getTime() const;
	float getTotalElapsedTime() const;
	float getTimeRemaining() const;
private:
	std::function<void()> callback;
	bool isTimerOn = false;
	float timeElapsed = 0.0f;
	int rolloverCount = 0;
};
