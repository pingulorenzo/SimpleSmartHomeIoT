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

#define LOG_MODULE "Humidity node"
#define LOG_LEVEL LOG_LEVEL_DBG
#define SERVER_EP ("coap://[fd00::1]:5683")
#define SERVER_REGISTRATION ("/registration")

#define HUM_MAX 90
#define HUM_MIN 30

extern coap_resource_t res_humidity;
extern coap_resource_t res_humidifier;
extern uint8_t humidifier_status;
extern uint8_t humidifier_value;
extern float humidity;