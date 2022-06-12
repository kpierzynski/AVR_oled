#ifndef __AVR_I2C_H_
#define __AVR_I2C_H_

#include <avr/io.h>

#define ACK	1
#define NACK	0

void i2c_init(uint16_t freq);
void i2c_write(uint8_t byte);
uint8_t i2c_read(uint8_t ack);
void i2c_start();
void i2c_stop();

#endif
