#ifndef _CURRENT_MEASUREMENT_H
#define _CURRENT_MEASUREMENT_H

#include "Arduino.h"

#include "serial_tx.h"
#include "device.h"

#include "debug_utils.h"
// #define DEBUG

#define PIN_CURRENT_MEASURE 36

#define N_SAMPLES 6000
#define VAL_REFERENCE 2048
#define CURRENT_STEP 0.01626

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t current_measure_timer;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void current_measure()
{
    float current;

    int val[N_SAMPLES];

    float max_val = 0;

    for (int i = 0; i < N_SAMPLES; i++)
    {
        val[i] = analogRead(PIN_CURRENT_MEASURE);

        // Busca el valor maximo medido
        if (max_val < val[i])
        {
            max_val = val[i];
        }
    }

    // Corrige posibles valores extremos
    if (max_val > 4096)
    {
        max_val = 4096;
    }

    if (max_val < 2048)
    {
        max_val = 0;
    }

    // calcula la corriente
    current = (max_val - VAL_REFERENCE) * CURRENT_STEP;

    String topic_current = (String)intensidadMotor + "=" + (String)current;

    // xQueueSend(queue_serial_tx, topic_current.c_str(), pdMS_TO_TICKS(100));

    if (elecVal.evDepGaloBajo || elecVal.evDepHuerto || elecVal.evCasa)
    {
        String topic_temp = (String)intensidadMotor + "=10";
        xQueueSend(queue_serial_tx, topic_temp.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }
    else
    {
        String topic_temp = (String)intensidadMotor + "=0";
        xQueueSend(queue_serial_tx, topic_temp.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));
    }

    DEBUG_PRINT((String)max_val);

    DEBUG_PRINT("Corriente electrica:");
    DEBUG_PRINT((String)current);
}

#endif // _CURRENT_MEASUREMENT_H
