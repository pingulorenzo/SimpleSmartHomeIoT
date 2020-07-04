#include "illumination_node.h"

static coap_message_type_t result = COAP_TYPE_RST;

PROCESS(illumination_node, "Illumination Node");
AUTOSTART_PROCESSES(&illumination_node);

static void response_handler(coap_message_t *response){

    if (response == NULL)
        return;
        
    LOG_DBG("Response %i\n", response->type);
    result = response->type;
}

PROCESS_THREAD(illumination_node, ev, data){

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    static struct etimer timer, timer_task;

    PROCESS_BEGIN();

    LOG_INFO("Starting light_level node \n");

    leds_set(LEDS_NUM_TO_MASK(LEDS_YELLOW));

    coap_activate_resource(&res_light_level, "sensors/light_level");
    coap_activate_resource(&res_light_source, "actuators/light_source");

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    do {

        coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
        coap_set_header_uri_path(request, (const char *) &SERVER_REGISTRATION);
        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
        
    } while (result == COAP_TYPE_RST);

    etimer_set(&timer, CLOCK_SECOND);
    etimer_set(&timer_task, CLOCK_SECOND);

    uint8_t direction = 1; // True = incrementale (Serve per la simulazione)

    while (1) {

        PROCESS_YIELD_UNTIL(etimer_expired(&timer) || ev == button_hal_press_event || etimer_expired(&timer_task));

        if (ev == button_hal_press_event)
            light_source_status = !light_source_status;
        else if (etimer_expired(&timer)){
            if (light_source_status)
                leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
            else
                leds_set(LEDS_NUM_TO_MASK(LEDS_RED));

            etimer_reset(&timer);
            
        } else {
            if (light_level == -1)
                init_value(light_level, LL_MIN, LL_MAX);

            // Simulo il passaggio della giornata
            if (direction){
                update_value_inc(light_level);
                light_level++;
            } else {
                update_value_dec(light_level);
                light_level--;
            }

            // Gestione overflow
            if (light_level >= LL_MAX){
                direction = 0;
                light_level = LL_MAX;
            } else if (light_level <= LL_MIN){        
                direction = 1;
                light_level = LL_MIN;
            }

            res_light_level.trigger();

            // Gestione automatica della luminosity
            if (!light_source_forced_status && light_source_status){
                light_source_value = LL_MAX - light_level; // Valore euristico
            }

            etimer_reset(&timer_task);
        }
    }

    PROCESS_END();
}
