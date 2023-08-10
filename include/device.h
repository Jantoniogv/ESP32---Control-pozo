#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "Arduino.h"

#include "lora_send.h"
#include "serial_tx.h"

// #define DEBUG
#include "debug_utils.h"

// Estado de cada electrovalvula
struct elecVal
{
    bool evDepGaloBajo = false;

    bool evDepHuerto = false;

    bool evCasa = false;

    bool motor = false;

    bool dev_fan = false;

} elecVal;

#define EV_DEP_GALO_BAJO 2
#define EV_DEP_HUERTO 15
#define EV_CASA 13

#define MOTOR 25

#define FAN 22

#define ON "ON"

#define OFF "OFF"

// Topic recibidos
const char power_motor[] = "motor/galo/bajo";

const char evDepGaloBajo[] = "deposito/galo/bajo";
const char evDepGaloBajoSec1[] = "regar/galo/bajo/sec1";
const char evDepGaloBajoSec2[] = "regar/galo/bajo/sec2";

const char evDepHuerto[] = "deposito/huerto";
const char evDepHuertoSec1[] = "regar/huerto/sec1";
const char evDepHuertoSec2[] = "regar/huerto/sec2";

const char evCasa[] = "deposito/casa";

const char dev_fan[] = "ventilador/pozo";

const char restart_pozo_galo_bajo[] = "pozo_galo_bajo/restart";
const char restart_valvulas_galo_bajo[] = "valvulas_galo_bajo/restart";
const char restart_nivel_dep_galo_bajo[] = "nivel_dep_galo_bajo/restart";

// Topic enviar estados
const char power_motor_state[] = "motor/galo/bajo/state";

const char evDepGaloBajoState[] = "deposito/galo/bajo/state";
const char evDepGaloBajoSec1State[] = "regar/galo/bajo/sec1/state";
const char evDepGaloBajoSec2State[] = "regar/galo/bajo/sec2/state";

const char evDepHuertoState[] = "deposito/huerto/state";
const char evDepHuertoSec1State[] = "regar/huerto/sec1/state";
const char evDepHuertoSec2State[] = "regar/huerto/sec2/state";

const char evCasaState[] = "deposito/casa/state";

const char dev_fan_state[] = "ventilador/pozo/state";

// Topic enviar
const char nivelDepGaloBajo[] = "nivel/deposito/galo/bajo";
const char nivelDepHuerto[] = "nivel/deposito/huerto";

// Topic enviar corriente consumida motor
const char intensidadMotor[] = "intensidad/motor";

// Topic enviar respuesta a peticion de reinicio y log de los dispositivos comunicados con este
const char restart_pozo_galo_bajo_state[] = "pozo_galo_bajo/restart/state";
const char restart_valvulas_galo_bajo_state[] = "valvulas_galo_bajo/restart/state";
const char restart_nivel_dep_galo_bajo_state[] = "nivel_dep_galo_bajo/restart/state";

void initPinDevice()
{
    // Inicializa pin motor
    pinMode(MOTOR, OUTPUT);
    digitalWrite(MOTOR, HIGH);

    // Inicializa pin dep. galo bajo
    pinMode(EV_DEP_GALO_BAJO, OUTPUT);
    digitalWrite(EV_DEP_GALO_BAJO, HIGH);

    // Inicializa pin dep. huerto
    pinMode(EV_DEP_HUERTO, OUTPUT);
    digitalWrite(EV_DEP_HUERTO, HIGH);

    // Inicializa pin casa
    pinMode(EV_CASA, OUTPUT);
    digitalWrite(EV_CASA, HIGH);

    // Inicializa pin ventilador
    pinMode(FAN, OUTPUT);
    digitalWrite(FAN, LOW);

    // Envia estados iniciales
    String state_init = (String)power_motor_state + "=OFF";
    xQueueSend(queue_serial_tx, state_init.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    state_init = (String)evDepGaloBajoState + "=OFF";
    xQueueSend(queue_serial_tx, state_init.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    state_init = (String)evDepHuertoState + "=OFF";
    xQueueSend(queue_serial_tx, state_init.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    state_init = (String)evCasaState + "=OFF";
    xQueueSend(queue_serial_tx, state_init.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    state_init = (String)dev_fan + "=OFF";
    xQueueSend(queue_serial_tx, state_init.c_str(), pdMS_TO_TICKS(QUEQUE_TEMP_WAIT));

    DEBUG_PRINT("Pines de las electrovalvulas, motor y ventilador iniciados...");

    // Escribe en el log inicializacion de los pines
    write_log("Pines de las electrovalvulas, motor y ventilador iniciados...");
}

#endif // _DEVICE_H_