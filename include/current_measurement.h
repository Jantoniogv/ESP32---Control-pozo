#ifndef _CURRENT_MEASUREMENT_H
#define _CURRENT_MEASUREMENT_H

#include "Arduino.h"

#include "serial_tx.h"
#include "device.h"
#include "log.h"

#include "debug_utils.h"
// #define DEBUG

#define PIN_CURRENT_MEASURE 36

#define N_SAMPLES 10000       // Numero de muestras tomadas
#define VAL_REFERENCE 2048    // 2048 es el valor teorico medido cuando la corriente es 0 y el voltaje seria 1,66 V, 0 seria el valor leido cuando la corriente pasa por el maximo negativo
#define CURRENT_STEP 0.01626  // Amperios cada punto leido analogico leido
#define FACTOR_RAIZ_DOS 0.707 // Valor resultante de dividir 1 entre raiz cuadrada de 2

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t current_measure_timer;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void current_measure()
{
    float current;

    int val;

    float max_val = 0;

    for (int i = 0; i < N_SAMPLES; i++)
    {
        val = analogRead(PIN_CURRENT_MEASURE);

        // Busca el valor maximo medido
        if (max_val < val)
        {
            max_val = val;
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
    current = (max_val - VAL_REFERENCE) * CURRENT_STEP * FACTOR_RAIZ_DOS;

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
    write_log("Max valor medido: " + (String)max_val);

    DEBUG_PRINT("Corriente electrica: " + (String)current);
    write_log("Corriente electrica: " + (String)current);
}

#endif // _CURRENT_MEASUREMENT_H
