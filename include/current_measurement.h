#ifndef _CURRENT_MEASUREMENT_H
#define _CURRENT_MEASUREMENT_H

#include "Arduino.h"

#include "serial_tx.h"
#include "device.h"
#include "log.h"

#include "debug_utils.h"
// #define DEBUG

#define PIN_CURRENT_MEASURE 36 // Pin toma datos analogicos

#define N_SAMPLES 2000        // Numero de muestras tomadas
#define N_SAMPLES_AVG 10      // Numero de muestras en un instante dado, a fin de tomar la media de estas 10 para una de cada muestra total
#define VAL_REFERENCE 1865    // 1860 es el valor teorico medido cuando la corriente es 0 y el voltaje seria 1,66 V
#define VAL_MAX 4096          // 4096 es el valor maximo que debe de medir el ADC
#define CURRENT_STEP 0.01626  // Amperios cada punto leido analogico leido
#define FACTOR_RAIZ_DOS 0.707 // Valor resultante de dividir 1 entre raiz cuadrada de 2

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t current_measure_timer;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void current_measure()
{
    float current;

    int max_val = 0;

    int val = 0;

    for (int i = 0; i < N_SAMPLES; i++)
    {

        for (int j = 0; j < N_SAMPLES_AVG; j++)
        {
            val += analogRead(PIN_CURRENT_MEASURE);
        }
        val = val / N_SAMPLES_AVG;

        // Busca el valor maximo medido
        if (max_val < val)
        {
            max_val = val;
        }

        DEBUG_PRINT(val);

        val = 0;

        vTaskDelay(pdMS_TO_TICKS(1));
    }

    // Corrige posibles valores extremos
    if (max_val > VAL_MAX)
    {
        max_val = VAL_MAX;
    }

    if (max_val < VAL_REFERENCE)
    {
        max_val = VAL_REFERENCE;
    }

    // calcula la corriente
    current = (max_val - VAL_REFERENCE) * CURRENT_STEP * FACTOR_RAIZ_DOS;

    DEBUG_PRINT((String)max_val);
    write_log("Max valor medido: " + (String)max_val);

    DEBUG_PRINT("Corriente electrica: " + (String)current);
    write_log("Corriente electrica: " + (String)current);
}

#endif // _CURRENT_MEASUREMENT_H

