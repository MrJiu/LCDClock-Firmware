/*
 * i2c.c
 *
 *  Created on: Sep 28, 2017
 *      Author: technix
 */

#include "i2c.h"

#include <stm32f1xx.h>
#include <stm32f1xx_it.h>
#include <stdbool.h>
#include <dreamos-rt/time.h>
#include <errno.h>
#include <stdio.h>

int i2c_open(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	I2C1->CR2 = (SystemCoreClock / 2) << I2C_CR2_FREQ_Pos;
	I2C1->CCR = (SystemCoreClock / 200000) << I2C_CCR_CCR_Pos;
	I2C1->TRISE = (SystemCoreClock / 2000000 + 1) << I2C_TRISE_TRISE_Pos;
	__DSB();

	I2C1->CR1 = I2C_CR1_PE;

	return 0;
}

static inline int i2c_start(uint8_t addr, bool reading)
{
	addr &= 0x7f;

	I2C1->SR1 = 0;
	I2C1->CR1 &= ~I2C_CR1_STOP;
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = (addr << 1) | (reading ? 1 : 0);
	while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));
	if (!(I2C1->SR2 & I2C_SR2_TRA) == !reading)
	{
		I2C1->CR1 |= I2C_CR1_STOP;
		return -EINVAL;
	}
	if (I2C1->SR1 & I2C_SR1_AF)
	{
		I2C1->CR1 |= I2C_CR1_STOP;
		return -I2C_ERROR_NACK;
	}

	return 0;
}

int i2c_transfer(uint8_t addr, const void *wbuf, size_t wlen, void *rbuf, size_t rlen)
{
	const uint8_t *wp = wbuf;
	uint8_t *rp = rbuf;

	if (wlen)
	{
		if (i2c_start(addr, false))
			return -I2C_ERROR_NACK;

		while ((const void *)wp - wbuf < wlen)
		{
			while (!(I2C1->SR1 & (I2C_SR1_TXE | I2C_SR1_AF)));
			if (I2C1->SR1 & I2C_SR1_AF)
			{
				I2C1->CR1 |= I2C_CR1_STOP;
				return -I2C_ERROR_NACK;
			}
			I2C1->DR = *wp;
			wp++;
		}
	}

	if (rlen || (!wlen && !rlen)) // Read is also used as detect
	{
		if (rlen)
			I2C1->CR1 |= I2C_CR1_ACK;

		if (i2c_start(addr, true))
			return -I2C_ERROR_NACK;

		while ((void *)rp - rbuf < rlen)
		{
			while (!(I2C1->SR1 & (I2C_SR1_RXNE | I2C_SR1_AF)));
			if (I2C1->SR1 & I2C_SR1_AF)
			{
				break;
			}
			*rp = I2C1->DR;
			rp++;
			if (!((void *)rp - rbuf < rlen - 1))
				I2C1->CR1 &= ~I2C_CR1_ACK;
		}
	}

	I2C1->CR1 |= I2C_CR1_STOP;
	return (void *)rp - rbuf;
}

void i2c_probe(void)
{
	fprintf(stderr, "0x00: .. .. .. .. .. .. .. .. ");
	for (uint8_t idx = 0x8; idx < 0x78; idx++)
	{
		if ((idx & 0xf) == 0)
			fprintf(stderr, "0x%02x: ", idx);

		int value = i2c_transfer(idx, NULL, 0, NULL, 0);

		if (value < 0)
			fprintf(stderr, "--");
		else
			fprintf(stderr, "%02x", idx);

		if ((idx & 0xf) == 0xf)
			fprintf(stderr, "\r\n");
		else
			fprintf(stderr, " ");
	}
	fprintf(stderr, ".. .. .. .. .. .. .. ..\r\n");
}

int i2c_close(void)
{
	I2C1->CR1 &= ~I2C_CR1_PE;
	__DSB();
	RCC->APB1ENR &= ~(RCC_APB1ENR_I2C1EN);
	__DSB();

	return 0;
}
