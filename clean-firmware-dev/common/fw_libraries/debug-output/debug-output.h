#ifndef _DEBUG_OUTPUT_H
#define _DEBUG_OUTPUT_H

#include <stdio.h>
#include <Arduino.h>

struct DEBUG
{
    static bool debug;
};

inline void print_debug(const char* format, ...)
{
    if(!DEBUG::debug) return;

    char str[128];
    va_list argptr;
    va_start(argptr, format);
    // vsnprintf_P(str, sizeof(str), format, argptr);
    vsnprintf(str, sizeof(str), format, argptr);
    va_end(argptr);
    Serial.println(str);
}

inline void print_debug(const __FlashStringHelper *format, ... )
{
    if(!DEBUG::debug) return;

    char str[128];
    va_list argptr;
    va_start(argptr, format);
    #ifdef __AVR__
        vsnprintf_P(str, sizeof(str), (const char *)format, argptr); // progmem for AVR
    #else
        vsnprintf(str, sizeof(str), (const char *)format, argptr); // for the rest of the world
    #endif
    va_end(argptr);
    Serial.println(str); 
}

#endif