#ifndef LOG_H
#define LOG_H

#include "stdio.h"

#define PROFILE_FUNC() (printf("[Function call] %s\n",__FUNCTION__))

#define LOG_INFO(...)                        (printf("[INFO]: "),  printf(__VA_ARGS__))
#define LOG_TRACE(...) (printf("\033[0;32m"), printf("[TRACE]: "), printf(__VA_ARGS__), printf("\033[0m"))
#define LOG_WARN(...)  (printf("\033[0;33m"), printf("[WARN]: "),  printf(__VA_ARGS__), printf("\033[0m"))
#define LOG_ERROR(...) (printf("\033[0;31m"), printf("[ERROR]: "), printf(__VA_ARGS__), printf("\033[0m"))

#endif