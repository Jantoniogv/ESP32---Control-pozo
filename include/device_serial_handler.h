#ifndef _DEVICE_SERIAL_HANDLER_H_
#define _DEVICE_SERIAL_HANDLER_H_

#include <Arduino.h>

#include "lora_send.h"
#include "device.h"
#include "serial_tx.h"
#include "config_init.h"
#include "log.h"

#include "debug_utils.h"
// #define DEBUG

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t start_motor_timer;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void start_motor()
{
    digitalWrite(MOTOR, LOW);
}

// Comprueba el tipo de mensage recibido y lo gestiona segun su funcion
void data_serial_receive_control(String data)
{
    String send_state = "";

    // Control del arranque del motor
    if (data.indexOf((String)power_motor) != -1)
    {
        String payload = data.substring(data.indexOf("=") + 1);

        DEBUG_PRINT(data);

        if (payload == ON && elecVal.motor == false)
        {
            DEBUG_PRINT("Motor=ON");

            // Arranca el motor
            xTimerStart(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

            // Marca el estado de la valvula
            elecVal.motor = true;
        }
        else if (payload == OFF)
        {
            DEBUG_PRINT("Motor=OFF");

            // Se para el motor y se asegura que el timer este parado
            xTimerStop(start_motor_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

            digitalWrite(MOTOR, HIGH);

            elecVal.motor = false;
        }

        send_state = (String)power_motor_state + "=" + payload;

        xQueueSend(queue_serial_tx, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

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

    // Reiniciar ESP32
    if (data.indexOf((String)restart_pozo_galo_bajo) != -1)
    {
        send_state = (String)restart_pozo_galo_bajo_state + "=OK";

        xQueueSend(queue_serial_tx, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

        vTaskDelay(pdTICKS_TO_MS(1000));

        ESP.restart();
    }

    // Enviar log
    if (data.indexOf((String)log_pozo_galo_bajo) != -1)
    {
        send_state = (String)log_pozo_galo_bajo_state + "=" + read_log();

        xQueueSend(queue_serial_tx, send_state.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }
}

#endif // _DEVICE_SERIAL_HANDLER_H_