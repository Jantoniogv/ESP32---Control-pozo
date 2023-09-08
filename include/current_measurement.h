#ifndef _CURRENT_MEASUREMENT_H
#define _CURRENT_MEASUREMENT_H

#include "Arduino.h"

#include "serial_tx.h"
#include "device.h"
#include "log.h"

// #define DEBUG
#include "debug_utils.h"

#define PIN_CURRENT_MEASURE 39 // Pin toma datos analogicos

#define N_SAMPLES 2000        // Numero de muestras tomadas
#define N_SAMPLES_AVG 10      // Numero de muestras en un instante dado, a fin de tomar la media de estas 10 para una de cada muestra total
#define VAL_REFERENCE 1860    // Es el valor teorico medido cuando la corriente es 0 y el voltaje seria 1,66 V
#define VAL_OFFSET 25         // Es el valor que compensa el error de medida debido a las interferencias inducidas por las cargas de las bobinas
#define CURRENT_STEP 0.01626  // Amperios cada punto leido analogico leido
#define FACTOR_RAIZ_DOS 0.707 // Valor resultante de dividir 1 entre raiz cuadrada de 2

// Declara le controlador del temporizador de la medida de la corriente
TimerHandle_t timer_current_measure;

// Se encarga de tomar la medida de la corriente y enviarla por el puerto serie
void current_measure()
{
    double current;

    int max_val = 0;

    int min_val = 2048;

    int val = 0;

    double val_cuadrado = 0;

    for (int i = 0; i < N_SAMPLES; i++)
    {

        /* for (int j = 0; j < N_SAMPLES_AVG; j++)
        {
            val += analogRead(PIN_CURRENT_MEASURE);
        }
        val = val / N_SAMPLES_AVG */
        ;

        for (int j = 0; j < N_SAMPLES_AVG; j++)
        {
            val += analogRead(PIN_CURRENT_MEASURE);
        }

        val = (val / N_SAMPLES_AVG);

        // Compensa la desviacion en la medida con un offset medido empiricamente
        if (val > VAL_REFERENCE)
        {
            val -= VAL_OFFSET;
        }
        else if (val < VAL_REFERENCE)
        {
            val += VAL_OFFSET;
        }

        // Busca el valor maximo medido
        if (max_val < val)
        {
            max_val = val;
        }

        // Busca el valor minimo medido
        if (min_val > val)
        {
            min_val = val;
        }

        val -= VAL_REFERENCE;

        val_cuadrado += val * val;

        // DEBUG_PRINT(val);

        val = 0;

        vTaskDelay(pdMS_TO_TICKS(1));
    }

    // Algoritmo para calcular el rms de la intensidad electrica medida
    current = sqrt(val_cuadrado / N_SAMPLES) * CURRENT_STEP;

    /* // Corrige posibles valores extremos
    if (max_val < VAL_REFERENCE)
    {
        max_val = VAL_REFERENCE;
    }

    // calcula la corriente
    current = (max_val - VAL_REFERENCE) * CURRENT_STEP * FACTOR_RAIZ_DOS; */

    // Envia los datos por el puerto serie
    String send_current = "";

    send_current = (String)intensidadMotor + "=" + String(current);

    xQueueSend(queue_serial_tx, send_current.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    // Registra los log y imprime en monitor serie en caso de debug
    DEBUG_PRINT("Max valor medido: " + (String)max_val);
    write_log("Max valor medido: " + (String)max_val);

    DEBUG_PRINT("Min valor medido: " + (String)min_val);
    write_log("Min valor medido: " + (String)min_val);

    DEBUG_PRINT("Corriente electrica: " + (String)current);
    write_log("Corriente electrica: " + (String)current);
}

#endif // _CURRENT_MEASUREMENT_H
