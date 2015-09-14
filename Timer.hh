#ifndef __TIMER_HH__
#define __TIMER_HH__

#include <chrono>

struct Timer
{
	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	void start()
	{
		t1 = std::chrono::high_resolution_clock::now();
	}
	double stop()
	{
		t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> difference_ = t2 - t1;
		double difference = std::chrono::duration_cast<std::chrono::microseconds>(difference_).count()*1.0e-6;
		return difference;
	}
};

#endif