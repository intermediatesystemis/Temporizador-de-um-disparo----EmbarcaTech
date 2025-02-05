#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN_BLUE 11
#define LED_PIN_RED 12
#define LED_PIN_GREEN 13
#define PUSH_BUTTON 5 

static volatile bool leds_active = false; 
static volatile uint32_t last_time = 0;

int64_t manipular_leds_callback(alarm_id_t id, void *user_data) {
    if (gpio_get(LED_PIN_BLUE)) {
        gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
        add_alarm_in_ms(3000, manipular_leds_callback, NULL, false);
    } else if (gpio_get(LED_PIN_RED)) {
        gpio_put(LED_PIN_RED, !gpio_get(LED_PIN_RED));
        add_alarm_in_ms(3000, manipular_leds_callback, NULL, false);
    } else {
        gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));
        leds_active = false;
    }

    return 0;
}

int main()
{
    stdio_init_all();

    gpio_init(PUSH_BUTTON);
    gpio_set_dir(PUSH_BUTTON, GPIO_IN);    
    gpio_pull_up(PUSH_BUTTON);

    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE,true);

    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED,true);

    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN,true);

    while (true) {
        uint32_t current_time = to_us_since_boot(get_absolute_time());

        if (gpio_get(PUSH_BUTTON) == 0 && current_time - last_time > 200000 && !leds_active) {

            gpio_put(LED_PIN_BLUE, !gpio_get(LED_PIN_BLUE));
            gpio_put(LED_PIN_RED, !gpio_get(LED_PIN_RED));
            gpio_put(LED_PIN_GREEN, !gpio_get(LED_PIN_GREEN));

            leds_active = true;

            add_alarm_in_ms(3000, manipular_leds_callback, NULL, false);
            last_time = current_time;
        }        
    }
}
