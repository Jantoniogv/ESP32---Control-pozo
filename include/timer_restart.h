#ifndef _RESET_TIMER_H_
#define _RESET_TIMER_H_

#include "Arduino.h"

TimerHandle_t timer_restart;

void restart_ESP32()
{
    ESP.restart();
}

#endif // _DEVICE_H_