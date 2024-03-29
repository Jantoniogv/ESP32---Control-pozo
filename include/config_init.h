#ifndef _CONFIGINIT_H_
#define _CONFIGINIT_H_
#include <Arduino.h>

#include <wifi.h>

#define TIME_TIMER_RESTART 5000
#define TIME_TIMER_START_MOTOR 17000
#define TIME_TIMER_MEASURE_CURRENT 10000

#define QUEQUE_TEMP_WAIT 5
#define SERIAL_TX_TEMP_WAIT 100

#define TIMER_START_STOP_WAIT 5

wifi_mode_t initWifiType = WIFI_MODE_APSTA;

String initSsidSTA = "DIGIFIBRA-Ap99";
String initSsidAP = "MyESP32AP_2022";

String initPassSTA = "612018_2482019";
String initPassAP = "adminadmin";

String initHost = "ESP32_2022";

String initIPap = "192.168.10.1";

String initIPsta = "";
String initGateway = "";
String initSubnet = "";

#endif //_CONFIGINIT_H_