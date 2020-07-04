#include "res_light_source.h"

uint8_t light_source_status = 0;
uint8_t light_source_forced_status = 0; // Se 1 il valore di light_source_value è custom, altrimenti automatico
uint16_t light_source_value = 0; // Valore di tensione di output

RESOURCE(res_light_source,
         "title=\"Illumination actuator\"; GET/PUT/POST; status=on|off&force=on|off&value=<value>; rt=\"Actuator\"\n",
         res_get_handler, res_post_put_handler, res_post_put_handler, NULL);

static void res_get_handler(coap_message_t *request, coap_message_t *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset){

    unsigned int accept = APPLICATION_JSON;
    coap_get_header_accept(request, &accept);

    if (accept == APPLICATION_JSON){
        coap_set_header_content_format(response, APPLICATION_JSON);

        if (!light_source_status)
            snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"status\":\"off\"}");
        else{
            char *res_forced_status = (!light_source_forced_status) ? "off" : "on";
            snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"status\":\"on\", \"force\":\"%s\", \"light_source_value\":%d}", res_forced_status, light_source_value);
        }
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
    uint8_t success= 1;

    len = coap_get_post_variable(request, "status", &value);
    
    if (len != 0){

        if (strncmp(value, "on", len) == 0)
            light_source_status = 1;
        else if (strncmp(value, "off", len) == 0)
            light_source_status = 0;
        else
            success = 0;
    } else
        success = 0;
    
    if (light_source_status){

        len = coap_get_post_variable(request, "force", &value);

        if (success && (len != 0)){

            if (strncmp(value, "on", len) == 0)
                light_source_forced_status = 1;
            else if (strncmp(value, "off", len) == 0)
                light_source_forced_status = 0;
            else
                success = 0;
        } else
            success = 0;
    }


    if (light_source_status && light_source_forced_status){

        len = coap_get_post_variable(request, "value", &value);

        if (success && (len != 0))
            light_source_value = atoi(value);
        else
            success = 0;
    }

    if (!success && light_source_forced_status)
        coap_set_status_code(response, BAD_REQUEST_4_00);
}