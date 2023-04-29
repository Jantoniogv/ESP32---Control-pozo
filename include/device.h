#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "Arduino.h"

#include "lora_send.h"

#include "debug_utils.h"
#define DEBUG

// Estado de cada electrovalvula
struct elecVal
{
    bool evDepGaloBajo = false;

    bool evDepHuerto = false;

    bool evCasa = false;

    bool motor = false;

} elecVal;

#define EV_DEP_GALO_BAJO 2
#define EV_DEP_HUERTO 15
#define EV_CASA 13

#define MOTOR 25

#define ON "ON"

#define OFF "OFF"

// Topic recibidos
const char evDepGaloBajo[] = "deposito/galo/bajo";
const char evDepGaloBajoSec1[] = "regar/galo/bajo/sec1";
const char evDepGaloBajoSec2[] = "regar/galo/bajo/sec2";

const char evDepHuerto[] = "deposito/huerto";
const char evDepHuertoSec1[] = "regar/huerto/sec1";
const char evDepHuertoSec2[] = "regar/huerto/sec2";

const char evCasa[] = "deposito/casa";

// Topic enviar estados
const char evDepGaloBajoState[] = "deposito/galo/bajo/state";
const char evDepGaloBajoSec1State[] = "regar/galo/bajo/sec1/state";
const char evDepGaloBajoSec2State[] = "regar/galo/bajo/sec2/state";

const char evDepHuertoState[] = "deposito/huerto/state";
const char evDepHuertoSec1State[] = "regar/huerto/sec1/state";
const char evDepHuertoSec2State[] = "regar/huerto/sec2/state";

const char evCasaState[] = "deposito/casa/state";

// Topic enviar
const char nivelDepGaloBajo[] = "nivel/deposito/galo/bajo";
const char nivelDepHuerto[] = "nivel/deposito/huerto";

// Topic enviar corriente consumida motor
const char intensidadMotor[] = "intensidad/motor";

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

    write_log("Pines de las electrovalvulas y motor iniciados...");

    DEBUG_PRINT("Pines de las electrovalvulas y motor iniciados...");
}

#endif // _DEVICE_H_