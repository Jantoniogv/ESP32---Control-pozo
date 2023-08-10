#ifndef _LORA_INIT_H
#define _LORA_INIT_H
#include <Arduino.h>

#include <SPI.h>
#include <LoRa.h>

#include "log.h"

// #define DEBUG
#include "debug_utils.h"

#define SCK 5   // GPIO5  -- SX1278's SCK
#define MISO 19 // GPIO19 -- SX1278's MISO
#define MOSI 27 // GPIO27 -- SX1278's MOSI
#define SS 18   // GPIO18 -- SX1278's CS
#define RST 23  // GPIO23 -- SX1278's RESET
#define DI0 26  // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND_LORA 433E6

void init_lora()
{
    SPI.begin(SCK, MISO, MOSI, SS);
    LoRa.setPins(SS, RST, DI0);

    if (!LoRa.begin(BAND_LORA))
    {
        DEBUG_PRINT("Inicio LoRa fallado!!");
        write_log("Inicio LoRa fallado!!");

        while (1)
            ;
    }

    DEBUG_PRINT("LoRa iniciado...");
    write_log("LoRa iniciado...");
}

#endif //_LORA_INIT_H