#include "res_conditioner.h"

uint8_t conditioner_status = 0;
uint8_t conditioner_temperature = 20; // Valore di default

RESOURCE(res_conditioner,
         "title=\"Conditioner actuator\"; GET/PUT/POST; status=on|off&target=<value>; rt=\"Actuator\"\n",
         res_get_handler, res_post_put_handler, res_post_put_handler, NULL);

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

    unsigned int accept = APPLICATION_JSON;
    coap_get_header_accept(request, &accept);

    if (accept == APPLICATION_JSON){
        coap_set_header_content_format(response, APPLICATION_JSON);
        char *res_status = (!conditioner_status) ? "off" : "on";

        if (!conditioner_status)
            snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"status\":\"%s\"}", res_status);
        else
            snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"status\":\"%s\", \"temperature\":%d}", res_status, conditioner_temperature);

        coap_set_payload(response, buffer, strlen((char *)buffer));
    } else {
        coap_set_status_code(response, NOT_ACCEPTABLE_4_06);
        const char *msg = "Supporting content-type application/json";
        coap_set_payload(response, msg, strlen(msg));
    }

    coap_set_header_max_age(response, MAX_AGE);
}

static void res_post_put_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

    size_t len = 0;
    const char *value = NULL;
    uint8_t success = 1;

    len = coap_get_post_variable(request, "status", &value);

    if (len != 0){

        if (strncmp(value, "on", len) == 0)
            conditioner_status = 1;
        else if (strncmp(value, "off", len) == 0)
            conditioner_status = 0;
        else
            success = 0;
    } else
        success = 0;

    if (conditioner_status){

        len = coap_get_post_variable(request, "value", &value);

        if (success && (len != 0))
            conditioner_temperature = atoi(value);
        else
            success = 0;
    }

    if (!success)
        coap_set_status_code(response, BAD_REQUEST_4_00);
}