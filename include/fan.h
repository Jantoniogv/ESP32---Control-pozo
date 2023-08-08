#ifndef _FAN_H_
#define _FAN_H_

#include <Arduino.h>

#include "log.h"
#include "device.h"

#include "debug_utils.h"
// #define DEBUG

#define FAN 23

void init_fan()
{
    pinMode(FAN, OUTPUT);

    DEBUG_PRINT("Pin del ventilador iniciado");
    write_log("Pin del ventilador iniciado");
}

void handler_fan(String status)
{
    if (status == ON)
    {
        digitalWrite(FAN, HIGH);
    }
    else if (status == OFF)
    {
        digitalWrite(FAN, LOW);
    }
}

#endif //_FAN_H_