/*
 * i2c.c
 *
 *  Created on: Sep 28, 2017
 *      Author: technix
 */

#include "i2c.h"

#include <stm32f3xx.h>
#include <stm32f303xc_it.h>

__attribute__((constructor(1000))) void i2c_init(void)
{
	RCC->CFGR3 |= RCC_CFGR3_I2C1SW | RCC_CFGR3_I2C2SW;
}

int i2c_open(void)
{

}

int i2c_transfer(uint8_t addr, const void *wbuf, size_t wlen, void *rbuf, size_t rlen)
{

}

int i2c_close(void)
{

}
