#include "utils.h"
#include <stdarg.h>
#include <stdio.h>

char tmp_buf[4096];
char *tprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsnprintf(tmp_buf, size(tmp_buf), fmt, args);
    va_end(args);
    return tmp_buf;
}
