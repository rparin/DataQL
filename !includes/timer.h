#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <ctime>
#include <time.h>
#include <chrono>

using namespace std;
class Timer {
private:
    clock_t _start;
    clock_t _stop;
public:
    Timer() :_start(clock()), _stop(clock()) {}
    void start() {
        _start = clock();
    }
    void stop() {
        _stop = clock();
    }
    double duration() {
        return (double(_stop) - double(_start)) / CLOCKS_PER_SEC;
    }
};

//using namespace std::chrono;
//using namespace std;
//
//class Timer {
//public:
//
//	//Constructor
//	Timer() : _start(clock()), _end(clock()){
//	}
//
//	//Start the timer
//	void start() {
//		_start = duration_cast<nanoseconds>
//			(system_clock::now().time_since_epoch()).count();
//	}
//
//	//End the timer
//	void end() {
//		_end = duration_cast<nanoseconds>
//			(system_clock::now().time_since_epoch()).count();
//	}
//
//	//Get duration
//	//Returns Microseconds
//	double duration() {
//		return static_cast<double>(_end - _start) 
//			/ CLOCKS_PER_SEC;
//	}
//	
//private:
//	clock_t _start;
//	clock_t _end;
//};

#endif //TIMER_H