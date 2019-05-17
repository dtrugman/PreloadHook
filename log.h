#pragma once

#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifndef NDEBUG
    #define LOG(fmt, ...) printf(fmt "\n", ## __VA_ARGS__)
    #define ERR(fmt, ...) printf(fmt ": %s (%d)\n", ## __VA_ARGS__, strerror(errno), errno)
#else
    #define LOG(fmt, ...)
    #define ERR(fmt, ...)
#endif
