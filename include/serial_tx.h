#ifndef _SERIAL_TX_H_
#define _SERIAL_TX_H_

#include <Arduino.h>

#include "serial_init.h"

#include "debug_utils.h"
#define DEBUG

void serial_tx(String data)
{
    write_log(data);

    SerialCom.print(data);
}

#endif //_SERIAL_TX_H_