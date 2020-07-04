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

#define LOG_MODULE "Illumination node"
#define LOG_LEVEL LOG_LEVEL_DBG
#define SERVER_EP ("coap://[fd00::1]:5683")
#define SERVER_REGISTRATION ("/registration")

#define LL_MAX 4095
#define LL_MIN 0

extern coap_resource_t res_light_level;
extern coap_resource_t res_light_source;
extern uint8_t light_source_status;
extern uint8_t light_source_forced_status;
extern uint16_t light_source_value;
extern int16_t light_level;