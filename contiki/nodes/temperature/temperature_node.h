#include "coap-blocking-api.h"
#include "coap-engine.h"
#include "contiki-net.h"
#include "contiki.h"
#include "dev/button-hal.h"
#include "dev/leds.h"
#include "sys/log.h"
#include "../utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MODULE "Temperature node"
#define LOG_LEVEL LOG_LEVEL_DBG
#define SERVER_EP ("coap://[fd00::1]:5683")
#define SERVER_REGISTRATION ("/registration")

#define TEMP_MAX 40
#define TEMP_MIN 16

extern coap_resource_t res_temperature;
extern coap_resource_t res_conditioner;
extern coap_resource_t res_radiator;
extern uint8_t conditioner_status;
extern uint8_t conditioner_temperature;
extern uint8_t radiator_status;
extern uint8_t radiator_temperature;
extern float temperature;