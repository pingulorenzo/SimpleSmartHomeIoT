#include "temperature_node.h"

static coap_message_type_t result = COAP_TYPE_RST;

PROCESS(temperature_node, "Temperature Node");
AUTOSTART_PROCESSES(&temperature_node);

static void response_handler(coap_message_t *response){

    if (response == NULL)
        return;

    LOG_DBG("Response %i\n", response->type);
    result = response->type;
}

PROCESS_THREAD(temperature_node, ev, data){

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    static struct etimer timer, timer_task;

    PROCESS_BEGIN();

    LOG_INFO("Starting temperature node\n");

    leds_set(LEDS_NUM_TO_MASK(LEDS_YELLOW));

    coap_activate_resource(&res_temperature, "sensors/temperature");
    coap_activate_resource(&res_conditioner, "actuators/conditioner");
    coap_activate_resource(&res_radiator, "actuators/radiator");

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    do {

        coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
        coap_set_header_uri_path(request, (const char *)&SERVER_REGISTRATION);
        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
        
    } while (result == COAP_TYPE_RST);

    etimer_set(&timer, CLOCK_SECOND);
    etimer_set(&timer_task, CLOCK_SECOND);

    while (1){

        PROCESS_YIELD_UNTIL(etimer_expired(&timer) || ev == button_hal_press_event || etimer_expired(&timer_task));

        if (ev == button_hal_press_event)
            conditioner_status = !conditioner_status;
        else if (etimer_expired(&timer)){

            if (conditioner_status || radiator_status)
                leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
            else
                leds_set(LEDS_NUM_TO_MASK(LEDS_RED));
            etimer_reset(&timer);

        } else {

            if (temperature == -1.0f)
                init_value(temperature, TEMP_MIN, TEMP_MAX);

            // Simulo la variazione dovuta alla non attuazione
            if (!conditioner_status && !radiator_status){
                update_value(temperature);
                if (temperature < TEMP_MIN)
                    temperature = TEMP_MIN;
                else if (temperature > TEMP_MAX)
                    temperature = TEMP_MAX;
                res_temperature.trigger();
            }

            // Simulo la variazione dovuta all'attuazione o eventuali spegnimenti automatici
            if (conditioner_status){
                if (temperature > conditioner_temperature){
                    update_value_dec(temperature);
                    res_temperature.trigger();
                } else
                    conditioner_status = 0;
            }

            if (radiator_status){
                if (temperature < radiator_temperature){
                    update_value_inc(temperature);
                    res_temperature.trigger();
                    }
                else
                    radiator_status = 0;
            }

            etimer_reset(&timer_task);
        }

    }

    PROCESS_END();
}