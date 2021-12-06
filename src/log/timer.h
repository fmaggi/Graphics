#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

#define TIME_IT() Timer engine_timer_unique_(__func__)

class Timer
{
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimeUnit = std::chrono::milliseconds;
    using Ratio = std::milli;
public:
    Timer(const std::string& functionName);
    ~Timer();
private:
    Clock::time_point start, end;
    std::string m_functionName;
};

#endif
