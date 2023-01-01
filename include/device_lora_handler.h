#ifndef _DEVICE_LORA_HANDLER_H_
#define _DEVICE_LORA_HANDLER_H_

#include <Arduino.h>

#include "serial_tx.h"
#include "device.h"

#include "debug_utils.h"
#define DEBUG

void data_lora_receive_control(String data)
{
    // Nivel del deposito galo bajo
    if (data.indexOf((String)nivelDepGaloBajo) != -1)
    {
        DEBUG_PRINT(data);

        serial_tx(data);
    }

    // Nivel del deposito huerto
    if (data.indexOf((String)nivelDepHuerto) != -1)
    {
        DEBUG_PRINT(data);

        serial_tx(data);
    }

    // Estados electrovalvulas depositos de riego
    if (data.indexOf((String)evDepHuertoSec1State) != -1)
    {
        DEBUG_PRINT(data);

        serial_tx(data);
    }

    if (data.indexOf((String)evDepHuertoSec2State) != -1)
    {
        DEBUG_PRINT(data);

        serial_tx(data);
    }

    if (data.indexOf((String)evDepGaloBajoSec1State) != -1)
    {
        DEBUG_PRINT(data);

        serial_tx(data);
    }

    if (data.indexOf((String)evDepGaloBajoSec2State) != -1)
    {
        DEBUG_PRINT(data);

        serial_tx(data);
    }
}

#endif //_DEVICE_LORA_HANDLER_H_