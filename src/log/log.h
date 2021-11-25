#ifndef LOG_H
#define LOG_H

#include "stdio.h"
#include "glm/glm.hpp"

#include <time.h>

// Returns the local date/time formatted as 2014-03-19 11:11:52. Not my function. Got it from stackoverflow
static inline char* getFormattedTime()
{

    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    static char _retval[20];
    strftime(_retval, sizeof(_retval), "%H:%M:%S", timeinfo);

    return _retval;
}

#define PROFILE_FUNC() (printf("[Function call] %s\n",__func__))

#define LOG(...)       (printf("[%s]: ", getFormattedTime()), printf(__VA_ARGS__))

#define LOG_INFO(...)                        (LOG(__VA_ARGS__))
#define LOG_TRACE(...) (printf("\033[0;32m"), LOG(__VA_ARGS__), printf("\033[0m\n")) // green
#define LOG_WARN(...)  (printf("\033[0;33m"), LOG(__VA_ARGS__), printf("\033[0m\n")) // yellow
#define LOG_ERROR(...) (printf("\033[0;31m"), LOG(__VA_ARGS__), printf("\033[0m\n")) // red

#ifdef DEBUG
    #define LOG_INFO_DEBUG(...)   (LOG_INFO(__VA_ARGS__))
    #define LOG_TRACE_DEBUG(...)  (LOG_TRACE(__VA_ARGS__))
    #define LOG_WARN_DEBUG(...)   (LOG_WARN(__VA_ARGS__))
    #define LOG_ERROR_DEBUG(...)  (LOG_ERROR(__VA_ARGS__))
#else
    #define LOG_INFO_DEBUG(...)
    #define LOG_TRACE_DEBUG(...)
    #define LOG_WARN_DEBUG(...)
    #define LOG_ERROR_DEBUG(...)
#endif

#define ASSERT(x, m) if (!(x)) { LOG_ERROR((m)); exit(-1); }

static inline void log_vec2(char* name, glm::vec2 v)
{
    LOG_INFO("%s: %.2f %.2f", name, v.x, v.y);
}

static inline void log_vec3(char* name, glm::vec3 v)
{
    LOG_INFO("%s: %.2f %.2f %.2f", name, v.x, v.y, v.z);
}

#endif
