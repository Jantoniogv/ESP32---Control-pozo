#ifndef _SERIAL_RX_H_
#define _SERIAL_RX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "device_serial_handler.h"
#include "log.h"

#include "debug_utils.h"
#define DEBUG

void serial_rx()
{
    if (SerialCom.available())
    {
        String data = SerialCom.readString();

        write_log(data);
        DEBUG_PRINT("SerialCom: " + data);

        data_serial_receive_control(data);
    }
}

#endif //_SERIAL_RX_H_