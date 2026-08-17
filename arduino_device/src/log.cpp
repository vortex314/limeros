/*
 * Log.cpp
 *
 *  Created on: Jul 3, 2016
 *      Author: lieven
 */
#include <log.h>
#include <Arduino.h>

Log::Log()
{
    _busy = false;
}
// log the file and line
Log &Log::logf(const char *format, ...)
{
    bool expected = false;
    if (_busy.compare_exchange_weak(expected, true))
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf("\n");
        expected = true;
        _busy.compare_exchange_weak(expected, false);
    }
    return *this;
}

#include <sys/time.h>
// log level-time-file-line
Log &Log::tfl(const char *lvl, const char *file, const uint32_t line)
{
    bool expected = false;
    if (_busy.compare_exchange_weak(expected, true))
    {
        uint64_t msec = millis();
        uint32_t sec = msec / 1000;
        uint32_t min = sec / 60;
        uint32_t hr = min / 60;
        uint32_t ms = msec % 1000;
        printf( "%s %2.2d:%2.2d:%2.2d.%3.3d | %15.15s:%4u | ", lvl,
                 (int)hr % 24,
                 (int)min % 60,
                 (int)sec % 60,
                 (int)ms,
                 file, (unsigned int)line);
        /*   struct tm* tm_info;
struct timeval tv;

gettimeofday(&tv, NULL);
tm_info = localtime(&tv.tv_sec);
strftime(buffer, 9, "%H:%M:%S", tm_info);
printf("%s.%03d\n", buffer, tv.tv_usec/1000);*/
        expected = true;
        _busy.compare_exchange_weak(expected, false);
    }
    return *this;
}

#include "driverlib/uart.h"

extern "C" void _write(int fd, char *ptr, int len)
{
    (void)fd;
    for (int i = 0; i < len; i++)
    {
        UARTCharPut(UART0_BASE, (unsigned char)ptr[i]);
  }
    // Serial.write((const unsigned char *)ptr, len);
    // Serial.flush();
}
