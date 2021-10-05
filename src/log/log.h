#ifndef LOG_H
#define LOG_H

#include "stdio.h"

#define PROFILE_FUNC() (printf("[Function call] %s\n",__func__))

#define LOG(...)                             (printf(__VA_ARGS__))
#define LOG_INFO(...)                        (printf("[INFO]: "),  printf(__VA_ARGS__))
#define LOG_TRACE(...) (printf("\033[0;32m"), printf("[TRACE]: "), printf(__VA_ARGS__), printf("\033[0m"))
#define LOG_WARN(...)  (printf("\033[0;33m"), printf("[WARN]: "),  printf(__VA_ARGS__), printf("\033[0m"))
#define LOG_ERROR(...) (printf("\033[0;31m"), printf("[ERROR]: "), printf(__VA_ARGS__), printf("\033[0m"))

#ifdef DEBUG
    #define LOG_DEBUG(...)        (LOG(__VA_ARGS__))
    #define LOG_INFO_DEBUG(...)   (LOG_INFO(__VA_ARGS__))
    #define LOG_TRACE_DEBUG(...)  (LOG_TRACE(__VA_ARGS__))
    #define LOG_WARN_DEBUG(...)   (LOG_WARN(__VA_ARGS__))
    #define LOG_ERROR_DEBUG(...)  (LOG_ERROR(__VA_ARGS__))
#else
    #define LOG_DEBUG(...)
    #define LOG_INFO_DEBUG(...)
    #define LOG_TRACE_DEBUG(...)
    #define LOG_WARN_DEBUG(...)
    #define LOG_ERROR_DEBUG(...)
#endif

#endif