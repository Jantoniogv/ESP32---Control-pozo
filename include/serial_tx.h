#ifndef _SERIAL_TX_H_
#define _SERIAL_TX_H_

#include <Arduino.h>

#include "serial_init.h"

#include "debug_utils.h"
#define DEBUG

QueueHandle_t queue_serial_tx = 0;

void serial_tx(void *pvParameter)
{

    char data_buffer[32] = {0};
    String data = "";
    for (;;)
    {
        if (xQueueReceive(queue_serial_tx, &data_buffer, pdMS_TO_TICKS(100)) == pdTRUE)
        {
            data = String(data_buffer);

            write_log(data);

            SerialCom.print(data + "\n");

            DEBUG_PRINT("Serial_com_send: " + data);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

#endif //_SERIAL_TX_H_