#include "humidity_node.h"

static coap_message_type_t result = COAP_TYPE_RST;

PROCESS(humidity_node, "Humidity Node");
AUTOSTART_PROCESSES(&humidity_node);

static void response_handler(coap_message_t *response){

    if (response == NULL)
        return;
        
    LOG_DBG("Response %i\n", response->type);
    result = response->type;
}

PROCESS_THREAD(humidity_node, ev, data) {

    static coap_endpoint_t server_ep;
    static coap_message_t request[1];
    static struct etimer timer, timer_task;

    PROCESS_BEGIN();

    LOG_INFO("Starting humidity node \n");

    leds_set(LEDS_NUM_TO_MASK(LEDS_YELLOW));

    coap_activate_resource(&res_humidity, "sensors/humidity");
    coap_activate_resource(&res_humidifier, "actuators/humidifier");

    coap_endpoint_parse(SERVER_EP, strlen(SERVER_EP), &server_ep);

    do {

        coap_init_message(request, COAP_TYPE_CON, COAP_GET, 0);
        coap_set_header_uri_path(request, (const char *) &SERVER_REGISTRATION);
        COAP_BLOCKING_REQUEST(&server_ep, request, response_handler);
        
    } while (result == COAP_TYPE_RST);

    etimer_set(&timer, CLOCK_SECOND);
    etimer_set(&timer_task, CLOCK_SECOND);

    while (1) {

        PROCESS_YIELD_UNTIL(etimer_expired(&timer) || ev == button_hal_press_event || etimer_expired(&timer_task));

        if (ev == button_hal_press_event)
            humidifier_status = !humidifier_status;        
        else if (etimer_expired(&timer)){

            if (humidifier_status)
                leds_set(LEDS_NUM_TO_MASK(LEDS_GREEN));
            else
                leds_set(LEDS_NUM_TO_MASK(LEDS_RED));

            etimer_reset(&timer);

        } else {

            if (humidity == -1)
                init_value(humidity, HUM_MIN, HUM_MAX);

            // Simulo la variazione dovuta alla non attuazione
            if (!humidifier_status){
                update_value(humidity);
                if (humidity < HUM_MIN)
                    humidity = HUM_MIN;
                else if (humidity > HUM_MAX)
                    humidity = HUM_MAX;
                res_humidity.trigger();
            }

            // Simulo la variazione dovuta all'attuazione            
            else if (humidity > humidifier_value){
                update_value_dec(humidity);
                res_humidity.trigger();
            } 

            // Gestisco lo spegnimento automatico
            if (humidifier_status && humidifier_value > humidity)
                humidifier_status = 0;

            etimer_reset(&timer_task);
        }
    }

    PROCESS_END();
}