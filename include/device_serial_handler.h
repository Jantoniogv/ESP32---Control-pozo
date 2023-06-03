#ifndef _DEVICE_SERIAL_HANDLER_H_
#define _DEVICE_SERIAL_HANDLER_H_

#include <Arduino.h>

#include "lora_send.h"
#include "device.h"
#include "serial_tx.h"
#include "config_init.h"

#include "debug_utils.h"
// #define DEBUG

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t start_motor_timer;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void start_motor()
{
    digitalWrite(MOTOR, LOW);
}

void data_serial_receive_control(String data)
{
    String send_state = "";

    // Control de las valvulas motorizadas del pozo
    if (data.indexOf((String)evDepGaloBajo) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.evDepGaloBajo == false)
        {
            DEBUG_PRINT("Dep_galo=ON");

            // Abre la valvula
            digitalWrite(EV_DEP_GALO_BAJO, LOW);

            // Marca el estado de la valvula
            elecVal.evDepGaloBajo = true;

            xTimerStart(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Dep_galo=OFF");

            // Para el motor en caso de que no haya otra valvula abierta
            if (elecVal.evCasa == false && elecVal.evDepHuerto == false)
            {
                // Se para el motor y se asegura que el timer este parado
                xTimerStop(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

                digitalWrite(MOTOR, HIGH);
            }

            // Cierra la valvula
            digitalWrite(EV_DEP_GALO_BAJO, HIGH);

            elecVal.evDepGaloBajo = false;
        }

        // serial_tx((String)evDepGaloBajoState + "=" + payload);

        send_state = (String)evDepGaloBajoState + "=" + payload;

        xQueueSend(queue_serial_tx, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    if (data.indexOf((String)evDepHuerto) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.evDepHuerto == false)
        {
            DEBUG_PRINT("Dep_huerto=ON");

            // Abre la valvula
            digitalWrite(EV_DEP_HUERTO, LOW);

            // Marca el estado de la valvula
            elecVal.evDepHuerto = true;

            xTimerStart(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Dep_huerto=OFF");

            // Para el motor en caso de que no haya otra valvula abierta
            if (elecVal.evCasa == false && elecVal.evDepGaloBajo == false)
            {
                // Se para el motor y se asegura que el timer este parado
                xTimerStop(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

                digitalWrite(MOTOR, HIGH);
            }

            // Cierra la valvula
            digitalWrite(EV_DEP_HUERTO, HIGH);

            elecVal.evDepHuerto = false;
        }

        // serial_tx((String)evDepHuertoState + "=" + payload);

        send_state = (String)evDepHuertoState + "=" + payload;

        xQueueSend(queue_serial_tx, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    if (data.indexOf((String)evCasa) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.evCasa == false)
        {
            DEBUG_PRINT("Casa=ON");

            // Abre la valvula
            digitalWrite(EV_CASA, LOW);

            // Marca el estado de la valvula
            elecVal.evCasa = true;

            xTimerStart(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Casa=OFF");

            // Para el motor en caso de que no haya otra valvula abierta
            if (elecVal.evDepGaloBajo == false && elecVal.evDepHuerto == false)
            {
                // Se para el motor y se asegura que el timer este parado
                xTimerStop(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

                digitalWrite(MOTOR, HIGH);
            }

            // Cierra la valvula
            digitalWrite(EV_CASA, HIGH);

            elecVal.evCasa = false;
        }

        // serial_tx((String)evCasaState + "=" + payload);

        send_state = (String)evCasaState + "=" + payload;

        xQueueSend(queue_serial_tx, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
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