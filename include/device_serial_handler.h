#ifndef _DEVICE_SERIAL_HANDLER_H_
#define _DEVICE_SERIAL_HANDLER_H_

#include <Arduino.h>

#include "lora_send.h"
#include "device.h"
#include "serial_tx.h"

#include "debug_utils.h"
#define DEBUG

void data_serial_receive_control(String data)
{
    // Control de las valvulas motorizadas del pozo
    if (data.indexOf((String)evDepGaloBajo) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON)
        {
            DEBUG_PRINT("Dep_galo=ON");
            digitalWrite(EV_DEP_HUERTO, HIGH);
            digitalWrite(EV_CASA, HIGH);
            vTaskDelay(1000);
            digitalWrite(MOTOR, HIGH);
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Dep_galo=OFF");
            digitalWrite(MOTOR, LOW);
            digitalWrite(EV_DEP_HUERTO, LOW);
            digitalWrite(EV_CASA, LOW);
        }
        serial_tx((String)evDepGaloBajoState + "=" + payload);
    }

    if (data.indexOf((String)evDepHuerto) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON)
        {
            DEBUG_PRINT("Dep_huerto=ON");
            digitalWrite(EV_DEP_GALO_BAJO, HIGH);
            digitalWrite(EV_CASA, HIGH);
            vTaskDelay(1000);
            digitalWrite(MOTOR, HIGH);
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Dep_huerto=OFF");
            digitalWrite(MOTOR, LOW);
            digitalWrite(EV_DEP_GALO_BAJO, LOW);
            digitalWrite(EV_CASA, LOW);
        }
        serial_tx((String)evDepHuertoState + "=" + payload);
    }

    if (data.indexOf((String)evCasa) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON)
        {
            DEBUG_PRINT("Casa=ON");
            digitalWrite(EV_DEP_HUERTO, HIGH);
            digitalWrite(EV_DEP_GALO_BAJO, HIGH);
            vTaskDelay(1000);
            digitalWrite(MOTOR, HIGH);
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Casa=OFF");
            digitalWrite(MOTOR, LOW);
            digitalWrite(EV_DEP_HUERTO, LOW);
            digitalWrite(EV_DEP_GALO_BAJO, LOW);
        }
        serial_tx((String)evCasaState + "=" + payload);
    }

    // Control de electrovalvulas del deposito de galo bajo
    if (data.indexOf((String)evDepGaloBajoSec1) != -1)
    {
        sendDataLora(data);
    }

    if (data.indexOf((String)evDepGaloBajoSec2) != -1)
    {
        sendDataLora(data);
    }

    // Control de electrovalvulas del deposito del huerto
    if (data.indexOf((String)evDepHuertoSec1) != -1)
    {
        sendDataLora(data);
    }

    if (data.indexOf((String)evDepHuertoSec2) != -1)
    {
        sendDataLora(data);
    }
}

#endif // _DEVICE_SERIAL_HANDLER_H_