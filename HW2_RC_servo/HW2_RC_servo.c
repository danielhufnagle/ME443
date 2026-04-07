#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 1
#define SERVO_DELAY 10 // use 10 for good smooth sweep (lower will cause servo to lag behind control too much, higher will cause steps to become too discrete)

void set_angle(int angle);

int main()
{
    stdio_init_all();

    // initialize PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    
    // set PWM to 50 Hz
    float div = 50; // must be between 0 and 255
    uint16_t wrap = 60000;
    pwm_set_clkdiv(slice_num, div);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(SERVO_PIN, 0);    

    while (true) {
        for (int i = 0; i <= 180; i++) {
            set_angle(i);
            sleep_ms(SERVO_DELAY);
        }
        for (int i = 180; i >= 0; i--) {
            set_angle(i);
            sleep_ms(SERVO_DELAY);
        }
    }
}

void set_angle(int angle) {
    // 0 degrees maps to 1.5% duty cycle, which is a pwm level of 900
    // 180 degrees maps to 12% duty cycle, which is a pwm level of 7200
    int level = 35 * angle + 900; // linear mapping between (0, 900) and (180, 7200)
    pwm_set_gpio_level(SERVO_PIN, level);
}
