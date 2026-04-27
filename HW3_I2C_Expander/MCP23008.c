#include <stdint.h>
#include "MCP23008.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"

void MCP23008_init(int sda, int scl) {
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
}

void pin_io(int pin, int direction) {
    uint8_t reg = 0x00;
    uint8_t val = 0;
    if (direction) {
        val |= (1 << pin);
    }
    else {
        val &= ~(1 << pin);
    }
    uint8_t buf[2] = {reg, val};
    int ret = i2c_write_blocking(I2C_PORT, MCP23008_ADDR, buf, 2, false);
}

void set_pin(int pin, int direction) {
    uint8_t reg = 0x0A;
    uint8_t val = 0;
    int ret = i2c_write_blocking(I2C_PORT, MCP23008_ADDR, &reg, 1, true);
    ret = i2c_read_blocking(I2C_PORT, MCP23008_ADDR, &val, 1, false);
    if (direction) {
        val |= (1 << pin);
    }
    else {
        val &= ~(1 << pin);
    }
    uint8_t buf[2] = {reg, val};
    ret = i2c_write_blocking(I2C_PORT, MCP23008_ADDR, buf, 2, false);
}

int read_pin(int pin) {
    uint8_t reg = 0x09;
    uint8_t val = 0;
    int ret = i2c_write_blocking(I2C_PORT, MCP23008_ADDR, &reg, 1, true);
    ret = i2c_read_blocking(I2C_PORT, MCP23008_ADDR, &val, 1, false);
    return (val & (1 << pin)) != 0;
}