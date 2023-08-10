#ifndef _DEVICE_LORA_HANDLER_H_
#define _DEVICE_LORA_HANDLER_H_

#include <Arduino.h>

#include "serial_tx.h"
#include "device.h"
#include "config_init.h"

// #define DEBUG
#include "debug_utils.h"

// void data_lora_receive_control(String data)
void data_lora_receive_control(String data)
{

    // Nivel del deposito galo bajo
    if (data.indexOf((String)nivelDepGaloBajo) != -1)
    {
        DEBUG_PRINT(data);

        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    // Nivel del deposito huerto
    if (data.indexOf((String)nivelDepHuerto) != -1)
    {
        DEBUG_PRINT(data);

        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    // Estados electrovalvulas depositos de riego
    if (data.indexOf((String)evDepHuertoSec1State) != -1)
    {
        DEBUG_PRINT(data);

        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    if (data.indexOf((String)evDepHuertoSec2State) != -1)
    {
        DEBUG_PRINT(data);

        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    if (data.indexOf((String)evDepGaloBajoSec1State) != -1)
    {
        DEBUG_PRINT(data);

        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    if (data.indexOf((String)evDepGaloBajoSec2State) != -1)
    {
        DEBUG_PRINT(data);

        xQueueSend(queue_serial_tx, data.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }
}

#endif //_DEVICE_LORA_HANDLER_H_