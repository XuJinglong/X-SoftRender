#pragma once

#include <Windows.h>
#include <iostream>

class Timer 
{
public:
	Timer() 
	{
		memset(&Freq, 0, sizeof(Freq));
		memset(&BeginCounter, 0, sizeof(BeginCounter));
		memset(&EndCounter, 0, sizeof(EndCounter));
	}
	~Timer() {}

	void Start() 
	{
		QueryPerformanceFrequency(&Freq);
		QueryPerformanceCounter(&BeginCounter);
	}
	double End() 
	{
		QueryPerformanceCounter(&EndCounter);
		double Duration = (double)(EndCounter.QuadPart - BeginCounter.QuadPart) / (double)Freq.QuadPart;
		std::cout << "持续时间:" << Duration << "秒" << std::endl;
		return Duration;
	}
private:
	LARGE_INTEGER Freq;
	LARGE_INTEGER BeginCounter;
	LARGE_INTEGER EndCounter;

};