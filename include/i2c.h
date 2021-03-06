/*
 * i2c.h
 *
 *  Created on: Sep 28, 2017
 *      Author: technix
 */

#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#include <stdint.h>
#include <stdbool.h>
#include <sys/cdefs.h>

#define I2C_ERROR_NACK		10000
#define I2C_ERROR_TIMEOUT	10001

__BEGIN_DECLS

int i2c_open(void);
void i2c_probe(void);
int i2c_transfer(uint8_t addr, const void *wbuf, size_t wlen, void *rbuf, size_t rlen);
int i2c_close(void);

__END_DECLS


#endif /* INCLUDE_I2C_H_ */
