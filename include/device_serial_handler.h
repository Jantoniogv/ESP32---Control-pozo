#ifndef _DEVICE_SERIAL_HANDLER_H_
#define _DEVICE_SERIAL_HANDLER_H_

#include <Arduino.h>

#include "lora_send.h"
#include "device.h"
#include "serial_tx.h"

#include "debug_utils.h"
#define DEBUG

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t start_motor_timer;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void start_motor()
{
    digitalWrite(MOTOR, HIGH);
}

void data_serial_receive_control(String data)
{
    // Control de las valvulas motorizadas del pozo
    if (data.indexOf((String)evDepGaloBajo) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.evDepGaloBajo == false)
        {
            DEBUG_PRINT("Dep_galo=ON");

            // Para el motor para asegurarse de hacer todas las maniobras con el parada, ya que puede estar en marcha de un trabajo anterior
            digitalWrite(MOTOR, LOW);

            digitalWrite(EV_DEP_HUERTO, HIGH);
            digitalWrite(EV_CASA, HIGH);

            elecVal.evDepGaloBajo = true;

            xTimerStart(start_motor_timer, 0);
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Dep_galo=OFF");

            digitalWrite(MOTOR, LOW);

            digitalWrite(EV_DEP_HUERTO, LOW);
            digitalWrite(EV_CASA, LOW);

            elecVal.evDepGaloBajo = false;
        }
        serial_tx((String)evDepGaloBajoState + "=" + payload);
    }

    if (data.indexOf((String)evDepHuerto) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.evDepHuerto == false)
        {
            DEBUG_PRINT("Dep_huerto=ON");

            // Para el motor para asegurarse de hacer todas las maniobras con el parada, ya que puede estar en marcha de un trabajo anterior
            digitalWrite(MOTOR, LOW);

            digitalWrite(EV_DEP_GALO_BAJO, HIGH);
            digitalWrite(EV_CASA, HIGH);

            elecVal.evDepHuerto = true;

            xTimerStart(start_motor_timer, 0);
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Dep_huerto=OFF");

            digitalWrite(MOTOR, LOW);

            digitalWrite(EV_DEP_GALO_BAJO, LOW);
            digitalWrite(EV_CASA, LOW);

            elecVal.evDepHuerto = false;
        }
        serial_tx((String)evDepHuertoState + "=" + payload);
    }

    if (data.indexOf((String)evCasa) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.evCasa == false)
        {
            DEBUG_PRINT("Casa=ON");

            // Para el motor para asegurarse de hacer todas las maniobras con el parada, ya que puede estar en marcha de un trabajo anterior
            digitalWrite(MOTOR, LOW);

            digitalWrite(EV_DEP_HUERTO, HIGH);
            digitalWrite(EV_DEP_GALO_BAJO, HIGH);

            elecVal.evCasa = true;

            xTimerStart(start_motor_timer, 0);
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Casa=OFF");

            digitalWrite(MOTOR, LOW);

            digitalWrite(EV_DEP_HUERTO, LOW);
            digitalWrite(EV_DEP_GALO_BAJO, LOW);

            elecVal.evCasa = false;
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