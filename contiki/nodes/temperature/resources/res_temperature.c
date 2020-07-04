#include "res_temperature.h"

float temperature = -1;
float last_temperature = -1;

EVENT_RESOURCE(
    res_temperature,
    "title=\"Temperature sensor\"; GET; rt=\"Sensor\"; obs\n",
    res_get_handler, NULL, NULL, NULL, res_event_handler);

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

    unsigned int accept = APPLICATION_JSON;
    coap_get_header_accept(request, &accept);

    if (accept == APPLICATION_JSON){
        coap_set_header_content_format(response, APPLICATION_JSON);
        snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"temperature\":%.1f}", temperature);
        coap_set_payload(response, buffer, strlen((char *)buffer));
    } else {
        coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
        const char *msg = "Supporting content-type application/json";
        coap_set_payload(response, msg, strlen(msg));
    }

    coap_set_header_max_age(response, MAX_AGE);
}

static void res_event_handler(){
    if (last_temperature != temperature){
        last_temperature = temperature;
        coap_notify_observers(&res_temperature);
    }
}