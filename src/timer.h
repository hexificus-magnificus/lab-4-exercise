#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <cstdint>

class Timer {
public:
	using Nanos = std::chrono::nanoseconds;
	using Micros = std::chrono::microseconds;
	using Millis = std::chrono::milliseconds;
	using Seconds = std::chrono::seconds;
	using Minutes = std::chrono::minutes;
	using Hours = std::chrono::hours;

	Timer()
		: t(std::chrono::steady_clock::now())
	{}

	void restart() {
		t = std::chrono::steady_clock::now();
	}

	template <typename T>
	uint64_t click() {
		auto now = std::chrono::steady_clock::now();
		uint64_t elapsed = std::chrono::duration_cast<T>(now-t).count();
		restart();
		return elapsed;
	}

	template <typename T>
	uint64_t glance() {
		auto now = std::chrono::steady_clock::now();
		uint64_t elapsed = std::chrono::duration_cast<T>(now-t).count();
		return elapsed;
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> t;

};

#endif  // TIMER_H
