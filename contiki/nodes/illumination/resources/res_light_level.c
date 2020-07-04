#include "res_light_level.h"

int16_t light_level = -1;
int16_t last_light_level = -1;

EVENT_RESOURCE(res_light_level,
		  "title=\"Illumination sensor\"; GET; rt=\"Sensor\"; obs\n",
		  res_get_handler, NULL, NULL, NULL, res_event_handler);

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

    unsigned int accept = APPLICATION_JSON;
    coap_get_header_accept(request, &accept);

    if (accept == APPLICATION_JSON){
        coap_set_header_content_format(response, APPLICATION_JSON);
        snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"light_level\":%d}", light_level);
        coap_set_payload(response, buffer, strlen((char *)buffer));
    } else {
        coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
        const char *msg = "Supporting content-type application/json";
        coap_set_payload(response, msg, strlen(msg));
    }

    coap_set_header_max_age(response, MAX_AGE);
}

static void res_event_handler(){

    if (last_light_level != light_level){
        last_light_level = light_level;
        coap_notify_observers(&res_light_level);
    }
}