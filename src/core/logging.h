#pragma once

#include "defines.h"

#define LOGGING_LEVEL_WARN_ENABLED 1
#define LOGGING_LEVEL_INFO_ENABLED 1

#ifdef DEBUG
#define LOGGING_LEVEL_DEBUG_ENABLED 1
#define LOGGING_LEVEL_TRACE_ENABLED 1
#endif

typedef enum logging_level {
    LOGGING_LEVEL_FATAL,
    LOGGING_LEVEL_ERROR,
    LOGGING_LEVEL_WARN,
    LOGGING_LEVEL_INFO,
    LOGGING_LEVEL_DEBUG,
    LOGGING_LEVEL_TRACE,
    LOGGING_LEVEL_MAX,
} logging_level;

S_API void log_output(logging_level level, const char* message, ...);

#ifndef S_FATAL
    #define S_FATAL(message, ...) log_output(LOGGING_LEVEL_FATAL, message, ##__VA_ARGS__);
#endif

#ifndef S_ERROR
    #define S_ERROR(message, ...) log_output(LOGGING_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif


#ifndef S_WARN
    #ifdef LOGGING_LEVEL_WARN_ENABLED
        #define S_WARN(message, ...) log_output(LOGGING_LEVEL_WARN, message, ##__VA_ARGS__);
    #else
        #define S_WARN(message, ...)
    #endif
#endif

#ifndef S_INFO
    #ifdef LOGGING_LEVEL_INFO_ENABLED
        #define S_INFO(message, ...) log_output(LOGGING_LEVEL_INFO, message, ##__VA_ARGS__);
    #else
        #define S_INFO(message, ...)
    #endif
#endif

#ifndef S_DEBUG
    #ifdef LOGGING_LEVEL_DEBUG_ENABLED
        #define S_DEBUG(message, ...) log_output(LOGGING_LEVEL_DEBUG, message, ##__VA_ARGS__);
    #else
        #define S_DEBUG(message, ...)
    #endif
#endif

#ifndef S_TRACE
    #ifdef LOGGING_LEVEL_TRACE_ENABLED
        #define S_TRACE(message, ...) log_output(LOGGING_LEVEL_TRACE, message, ##__VA_ARGS__);
    #else
        #define S_TRACE(message, ...)
    #endif
#endif