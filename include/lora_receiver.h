#ifndef _LORA_RECEIVER_H_
#define _LORA_RECEIVER_H_
#include <Arduino.h>

#include "device_lora_handler.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

/* void testMem(String taskName)
{
    static int stackMem, stackMemTemp;

    stackMemTemp = uxTaskGetStackHighWaterMark(nullptr);

    if (!stackMem || (stackMemTemp < stackMem))
    {
        stackMem = stackMemTemp;
        Serial.printf("\n%s tiene stack mem %u\n", taskName.c_str(), stackMem);
    }
} */

void receiveDataLora(void *pvParameter)
{
    DEBUG_PRINT("Lora receiver");

    for (;;)
    {
        String packetData = "";

        int packetSize = LoRa.parsePacket();

        if (packetSize)
        {
            // received a packet
            DEBUG_PRINT("Received packet => Pack size: " + (String)packetSize + " RSSI: " + (String)LoRa.packetRssi());

            // read packet
            while (LoRa.available())
            {
                packetData += (char)LoRa.read();
            }

            DEBUG_PRINT("LoRa rec: " + packetData);
            write_log("LoRa rec: " + packetData);

            // Procesa los datos recibidos
            data_lora_receive_control(packetData);
        }

        // Calcula la memoria libre que queda de esta tarea, a fin de calcular la memoria necesaria para ella
        // testMem("receiveDataLora");

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

#endif //_LORA_RECEIVER_H_