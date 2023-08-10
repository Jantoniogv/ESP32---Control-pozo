#ifndef _SERIAL_RX_H_
#define _SERIAL_RX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "device_serial_handler.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

void serial_rx()
{
    if (SerialCom.available())
    {
        String data = SerialCom.readStringUntil('\n');

        DEBUG_PRINT("Serial_com_rec: " + data);
        write_log("Serial_com_rec: " + data);

        data_serial_receive_control(data);
    }
}

#endif //_SERIAL_RX_H_