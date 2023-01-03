#include <Arduino.h>

#include <SPI.h>
// #include <Wire.h>

#include "config.h"
#include "serial_init.h"
#include "wifi_functions.h"
#include "server_functions.h"
#include "lora_init.h"
#include "lora_receiver.h"
#include "display_oled.h"
#include "serial_rx.h"
#include "device.h"
#include "device_serial_handler.h"
#include "current_measurement.h"

#include "debug_utils.h"
#define DEBUG

void setup()
{
  // Borra todas las claves del espacio de nombres en memoria
  // eraseFlash("config");

  // Inicializa el objeto con la variables de configuracion
  Config configData;

  // Inicia la conexion serial
  serial_init();

  DEBUG_PRINT("Serial iniciado...");

  // Inicia el modulo LoRa
  init_lora();

  DEBUG_PRINT("LoRa iniciado...");

  // Inicia el display OLED
  // init_oled();

  // Iniciamos los temporizadores encargados de reconectar la conexion wifi y mqtt, en caso de desconexion
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(wifiConnectSTA));

  // Se captura los eventos de la conexion wifi
  WiFi.onEvent(WiFiEvent);

  // Configuramos el modo del wifi
  WiFi.mode(configData.getWifiType());

  Serial.println("wifi");

  // Iniciamos la conexion wifi como punto de acceso
  wifiConnectAP();

  // Iniciamos la conexion wifi como cliente
  wifiConnectSTA();

  // Muestra los datos de conexion al punto de acceso
  // printDataWifiAP(configData);

  // Configura el servidor web
  serverHandlers();
  DEBUG_PRINT("server");

  // Inicia ElegantOTA
  AsyncElegantOTA.begin(&server);

  // Inicia el servidor
  server.begin();

  write_log("Servidor HTTP iniciado...");

  DEBUG_PRINT("Servidor HTTP iniciado...");

  // Inicia la tarea que recibe los datos por LoRa
  xTaskCreatePinnedToCore(receiveDataLora, "loraData", 2048, nullptr, 5, nullptr, 1);

  // Iniciamos el temporizador encargado de medir la corriente del motor del pozo periodicamente
  start_motor_timer = xTimerCreate("start_motor_timer", pdMS_TO_TICKS(17000), pdFALSE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(start_motor));

  // Iniciamos el temporizador encargado de medir la corriente del motor del pozo periodicamente
  current_measure_timer = xTimerCreate("current_measure", pdMS_TO_TICKS(5000), pdTRUE, (void *)0, reinterpret_cast<TimerCallbackFunction_t>(current_measure));
  xTimerStart(current_measure_timer, 0);

  write_log("Temporizador de medicion de la corriente iniciado...");

  // Inicia los pines usados para activas los dispositivos
  initPinDevice();
}

void loop()
{
  serial_rx();
}
