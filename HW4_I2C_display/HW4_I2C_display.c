#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "ssd_text.h"

#define ADC_PIN 26
#define HEARTBEAT_INTERVAL 1000000
#define HEARTBEAT_PIN 0

char message_1[50];
char message_2[50];
float adc_volts;
uint16_t adc_val;
uint64_t curr_time_since_boot;
uint64_t prev_time_since_boot = 0;
uint64_t prev_heartbeat;
int heartbeat_state = 0;

int main()
{
    stdio_init_all();

    // setup heartbeat gpio
    gpio_init(HEARTBEAT_PIN);
    gpio_set_dir(HEARTBEAT_PIN, 1);

    // set up i2c instance
    i2c_init(i2c0, 400000);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);

    // set up display
    ssd1306_setup();
    ssd1306_clear();

    // set up adc
    adc_init();
    adc_gpio_init(ADC_PIN);

    // prepare heartbeat
    prev_heartbeat = time_us_64();

    while (true) {

        // read from adc
        adc_val = adc_read();
        adc_volts = 3.3 * (float)adc_val / 4095.0;

        // get framerate
        curr_time_since_boot = to_us_since_boot(get_absolute_time());
        uint64_t time_since_last_frame = curr_time_since_boot - prev_time_since_boot;
        prev_time_since_boot = curr_time_since_boot;
        uint64_t fps = 1000000 / time_since_last_frame;

        // display text
        sprintf(message_1, "ADC0: %f", adc_volts);
        sprintf(message_2, "FPS: %ld", fps);

        draw_string(2, 2, message_1);
        draw_string(2, 22, message_2);

        // heartbeat
        if (time_us_64() - prev_heartbeat >= HEARTBEAT_INTERVAL) {
            prev_heartbeat = time_us_64();
            if (heartbeat_state == 0) {
                heartbeat_state = 1;
            }
            else {
                heartbeat_state = 0;
            }
            gpio_put(HEARTBEAT_PIN, heartbeat_state);
        }
    }
}
