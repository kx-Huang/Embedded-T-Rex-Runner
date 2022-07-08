#ifndef I2C_H_
#define I2C_H_

#include "msp.h"
#include <stdint.h>

void i2c_init(void);
uint16_t i2c_read_16(uint8_t slave_address, uint8_t dev_address);
void i2c_write_16(uint8_t slave_address, uint8_t dev_address, uint16_t data);

#endif /* I2C_H_ */
