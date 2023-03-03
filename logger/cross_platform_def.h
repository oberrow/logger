#pragma once
#ifdef _WIN32
#else
#include <stdio.h>
#include <errno.h>
#define __crt_va_start va_start
#define __crt_va_end va_end
#endif