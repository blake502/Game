#pragma once

#include "defines.h"

#ifdef DEBUG
#define DEBUG_ASSERTIONS_ENABLED
#endif

#ifdef WINDOWS
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define S_ASSERT(expr)                                              \
{                                                                   \
    if(expr) {                                                      \
    } else {                                                        \
        report_assertion_failure(#expr, "", __FILE__, __LINE__);    \
        debugBreak();                                               \
    }                                                               \
}

#define S_ASSERT_MSG(expr, message)                                      \
{                                                                        \
    if(expr) {                                                           \
    } else {                                                             \
        report_assertion_failure(#expr, message, __FILE__, __LINE__);    \
        debugBreak();                                                    \
    }                                                                    \
}

#ifdef DEBUG_ASSERTIONS_ENABLED
    #define S_ASSERT_DEBUG(expr) S_ASSERT(expr)
    #define S_ASSERT_DEBUG_MSG(expr, message) S_ASSERT_MSG(expr, message)
#else
    #define S_ASSERT_DEBUG(expr)
    #define S_ASSERT_DEBUG_MSG(expr, message)
#endif