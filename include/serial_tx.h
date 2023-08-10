#ifndef _SERIAL_TX_H_
#define _SERIAL_TX_H_

#include <Arduino.h>

#include "serial_init.h"
#include "config_init.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

QueueHandle_t queue_serial_tx = 0;

void serial_tx(void *pvParameter)
{

    char data_buffer[32] = {0};
    String data = "";
    for (;;)
    {
        if (xQueueReceive(queue_serial_tx, &data_buffer, pdMS_TO_TICKS(QUEQUE_TEMP_WAIT)) == pdTRUE)
        {
            data = String(data_buffer);

            // Borra el buffer en caso de tener algun byte corrupto
            while (SerialCom.available())
            {
                SerialCom.read();
            }

            SerialCom.print("__recolector__basura__\n");

            vTaskDelay(pdMS_TO_TICKS(SERIAL_TX_TEMP_WAIT));

            SerialCom.print(data + "\n");

            DEBUG_PRINT("Serial_com_send: " + data);
            write_log("Serial_com_send: " + data);
        }

        vTaskDelay(pdMS_TO_TICKS(SERIAL_TX_TEMP_WAIT));
    }
}

#endif //_SERIAL_TX_H_