#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "MCP23008.h"

#define I2C_SDA 8
#define I2C_SCL 9

int main()
{
    stdio_init_all();
    MCP23008_init(I2C_SDA, I2C_SCL);
    pin_io(7, PIN_OUTPUT);
    pin_io(0, PIN_INPUT);
    while (true) {
        int button = read_pin(0);
        if (button == 0) {
            set_pin(7, 1);
        }
        else {
            set_pin(7, 0);
        }
    }
}
