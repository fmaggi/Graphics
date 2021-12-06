#include "timer.h"

#include <iostream>

#include "log.h"

Timer::Timer(const std::string& functionName)
    : m_functionName(functionName)
{
    start = Clock::now();
}

Timer::~Timer()
{

    end = Clock::now();
    std::chrono::duration<float, Ratio> dif = end - start;

    LOG_INFO("%s: %f ms", m_functionName.c_str(), dif.count());
}
