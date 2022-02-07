#pragma once
#include <chrono>

class Timer 
{
public:
	void Start() 
	{
		mStartTime = std::chrono::system_clock::now();
		mIsRunning = true;
	}

	void Stop()
	{
		mEndTime = std::chrono::system_clock::now();
		mIsRunning = false;
	}

	double GetElapsedMilliseconds() 
	{
		std::chrono::time_point<std::chrono::system_clock> endTime;

		if (mIsRunning)
		{
			endTime = std::chrono::system_clock::now();
		}
		else
		{
			endTime = mEndTime;
		}

		return std::chrono::duration<double>(endTime - mStartTime).count() * 1000;
	}


private:
	std::chrono::time_point<std::chrono::system_clock> mStartTime;
	std::chrono::time_point<std::chrono::system_clock> mEndTime;

	bool mIsRunning = false;
};