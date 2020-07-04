#include "contiki.h"
#include "coap-engine.h"
#include "sys/log.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOG_MODULE "Humidity sensor"
#define LOG_LEVEL LOG_LEVEL_INFO
#define MAX_AGE 60

static void res_get_handler(coap_message_t *, coap_message_t *, uint8_t *, uint16_t, int32_t *);
static void res_event_handler();

extern uint8_t humidifier_status;
extern uint8_t humidifier_value;