/*
 * i2c.c
 *
 *  Created on: Sep 28, 2017
 *      Author: technix
 */

#include "i2c.h"

#include <stm32f3xx.h>
#include <stm32f303xc_it.h>
#include <stdbool.h>
#include <dreamos-rt/time.h>

__attribute__((constructor(1000))) void i2c_init(void)
{
	RCC->CFGR3 |= RCC_CFGR3_I2C1SW | RCC_CFGR3_I2C2SW;
}

int i2c_open(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	__DSB();
	I2C1->CR1 = I2C_CR1_DFN;
	I2C1->TIMINGR = 0x60202639; // Magic number from STM32CubeMX.
	__DSB();
	I2C1->CR1 |= I2C_CR1_PE;
	__DSB();

	return 0;
}

static inline int i2c_start(uint8_t addr, bool reading)
{
	I2C1->ICR = I2C_ICR_NACKCF;
	SET_FIELD(I2C1->CR2, I2C_CR2_RD_WRN | I2C_CR2_SADD, (reading ? I2C_CR2_RD_WRN : 0) | (addr & 0x7f) << 1);
	__DSB();
	I2C1->CR2 |= I2C_CR2_START;
	__DSB();
	while (!(I2C1->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)))
	{
		yield();
		__DSB();
	}
	if (I2C1->ISR & I2C_ISR_NACKF)
		return -I2C_ERROR_NACK;
	return 0;
}

int i2c_transfer(uint8_t addr, const void *wbuf, size_t wlen, void *rbuf, size_t rlen)
{
	const uint8_t *wp = wbuf;
	uint8_t *rp = rbuf;
	bool active = false;

	if (wlen)
	{
		while (wlen > 0xff)
		{
			// Write out 0xff bytes
			I2C1->CR2 = I2C_CR2_RELOAD | (0xff << 16);
			wlen -= 0xff;
			if (!active)
			{
				active = true;
				if (i2c_start(addr, false))
					return -I2C_ERROR_NACK;
			}
		}
	}

	active = false;

	if (rlen || (!wlen && !rlen)) // Read is also used as detect
	{
		if (!active)
		{
			active = true;
			if (i2c_start(addr, false))
				return -I2C_ERROR_NACK;
		}
	}

	I2C1->CR2 |= I2C_CR2_STOP;

	return (void *)rp - rbuf;
}

int i2c_close(void)
{
	I2C1->CR1 &= ~I2C_CR1_PE;
	__DSB();
	RCC->APB1ENR &= ~(RCC_APB1ENR_I2C1EN);
	__DSB();

	return 0;
}
