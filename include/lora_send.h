#ifndef _LORA_SEND_H_
#define _LORA_SEND_H_
#include <Arduino.h>

#include "lora_init.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

void sendDataLora(String data)
{
    DEBUG_PRINT(data);

    // send packet
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();

    DEBUG_PRINT("LoRa send: " + data);
    write_log("LoRa send: " + data);
}

#endif //_LORA_SEND_H_