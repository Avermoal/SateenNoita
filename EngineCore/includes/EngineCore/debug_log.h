#ifndef ENGINECORE_DEBUG_LOG_H
#define ENGINECORE_DEBUG_LOG_H


#ifdef NDEBUG
#define LOG_CRITICAL(...)
#else
#include <stdio.h>
#define LOG_CRITICAL(...) printf("[ERROR]:"); printf(__VA_ARGS__)
#endif

#endif/*ENGINECORE_DEBUG_LOG_H*/
