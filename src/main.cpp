#include <Arduino.h>

#include <SPI.h>
// #include <Wire.h>

#include "config.h"
#include "timer_restart.h"
#include "serial_init.h"
#include "wifi_functions.h"
#include "server_functions.h"
#include "lora_init.h"
#include "lora_receiver.h"
#include "serial_rx.h"
#include "serial_tx.h"
#include "device.h"
#include "device_serial_handler.h"
#include "current_measurement.h"

#include "debug_utils.h"
// #define DEBUG

void setup()
{
  // Borra todas las claves del espacio de nombres en memoria
  // eraseFlash("config");

  // Inicializa el objeto con la variables de configuracion
  Config configData;

  // Inicia la conexion serial
  serial_init();

  // Inicia el modulo LoRa
  init_lora();

  // Se captura los eventos de la conexion wifi
  WiFi.onEvent(WiFiEvent);

  // Configuramos el modo del wifi
  WiFi.mode(configData.getWifiType());

  // Iniciamos la conexion wifi como punto de acceso
  wifiConnectAP();

  // Inicia y configura el servidor web
  init_server();

  // Inicia la cola que almacena los datos a enviar por el puerto serie
  queue_serial_tx = xQueueCreate(20, sizeof(char) * 32);

  // Inicia la tarea que envia los datos por el puerto serie
  xTaskCreatePinnedToCore(serial_tx, "serial_tx", 2048, nullptr, 0, nullptr, 1);

  // Inicia la tarea que recibe los datos por LoRa
  xTaskCreatePinnedToCore(receiveDataLora, "loraData", 2048, nullptr, 0, nullptr, 1);

  // Iniciamos el temporizador encargado de medir la corriente del motor del pozo periodicamente
  timer_restart = xTimerCreate("timer_restart", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(restart_ESP32));

  // Iniciamos el temporizador encargado de medir la corriente del motor del pozo periodicamente
  start_motor_timer = xTimerCreate("start_motor_timer", pdMS_TO_TICKS(17000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(start_motor));

  // Iniciamos el temporizador encargado de medir la corriente del motor del pozo periodicamente
  current_measure_timer = xTimerCreate("current_measure", pdMS_TO_TICKS(10000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(current_measure));
  xTimerStart(current_measure_timer, pdMS_TO_TICKS(TIMER_START_STOP_WAIT));

  write_log("Temporizador de medicion de la corriente iniciado...");

  // Inicia los pines usados para activas los dispositivos
  initPinDevice();

  // Iniciamos la conexion wifi como cliente una vez iniciada todos los procesos a fin de evitar problemas en caso de que la red WiFi no este disponible
  wifiConnectSTA();
}

void loop()
{
  serial_rx();

  /*    if (Serial.available())
     {
       String data = Serial.readString();

       data_serial_receive_control(data);
     } */

  vTaskDelay(pdMS_TO_TICKS(1));
}
