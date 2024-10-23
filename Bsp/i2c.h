#ifndef __I2C_H
#define __I2C_H

#include "gd32f10x.h"

#define I2C_SPEED      100000
#define I2C0_OWN_ADDR  0xA0

void i2c_init(void);      // ≥ı ºªØi2c0

#endif
