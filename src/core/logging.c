#include "logging.h"
#include "assert.h"
#include "platform\platform.h"

//TODO: Temp
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

S_API void log_output(logging_level level, const char* message, ...)
{
    const char* level_strings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};

    b8 is_error = level <= LOGGING_LEVEL_ERROR;

    const i32 msg_length = 32000;
    char in_message[msg_length];
    platform_setmem(in_message, 0, sizeof(in_message));

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(in_message, msg_length, message, arg_ptr);
    va_end(arg_ptr);

    char out_message[msg_length];
    platform_setmem(out_message, 0, sizeof(out_message));
    
    sprintf(out_message, "%s%s\n", level_strings[level], in_message);

    if(is_error)
        platform_console_write_error(out_message, level);
    else
        platform_console_write(out_message, level);

}

void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
    log_output(LOGGING_LEVEL_FATAL, "Assertion Failure at [%s:%i]\n%s\n%s", file, line, expression, message);
    //log_output(LOGGING_LEVEL_FATAL, "%s  %s %s %s", expression, message, file, line);
    //log_output(LOGGING_LEVEL_FATAL, "%s", message);
}