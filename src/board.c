/*
 * board.c
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#include "board.h"

#include <dreamos-rt/gpio.h>
#include <stdio.h>
#include <stm32f3xx.h>

__attribute__((constructor(1001))) void board_init(void)
{
	static const int pinModes[] =
	{
			// GPIOA
			OUTPUT,					// LCD_D0
			OUTPUT,					// LCD_D1
			OUTPUT,					// LCD_D2
			OUTPUT,					// LCD_D3
			OUTPUT,					// LCD_D4
			OUTPUT,					// LCD_D5
			OUTPUT,					// LCD_D6
			OUTPUT,					// LCD_D7

			OUTPUT,					// USB_PU
			AFIO_PU(GPIO_MODE_AF7),	// DBG_TX
			AFIO_PU(GPIO_MODE_AF7),	// DBG_RX
			AFIO(GPIO_MODE_AF14),	// USB_DM
			AFIO(GPIO_MODE_AF14),	// USB_DP
			-1,						// DBG_TMS
			-1,						// DBG_TCK
			-1,						// DBG_TDI

			// GPIOB
			OUTPUT,					// LCD_RS
			ANALOG,					// ADC_BATT
			OUTPUT,					// LCD_RW
			-1,						// DBG_TDO
			-1,						// DBG_TRST
			OUTPUT,					// WL_RESET
			AFIO_PU(GPIO_MODE_AF5),	// LCD_BL
			OUTPUT,					// WL_EN

			AFIO_OD(GPIO_MODE_AF4),	// I2C_SCL
			AFIO_OD(GPIO_MODE_AF4),	// I2C_SDA
			AFIO_PU(GPIO_MODE_AF7),	// WL_TX
			AFIO_PU(GPIO_MODE_AF7),	// WL_RX
			OUTPUT,					// WL_BOOT
			OUTPUT,					// LCD_EN
			INPUT_PULLUP,			// ACK_BTN
			INPUT					// USB_VBUS
	};

	for (uint8_t pin = 0; pin < 32; pin++)
	{
		int mode = pinModes[pin];
		if (mode > 0)
			pinMode(pin, mode);
	}
}

__attribute__((section(".ccm"))) const uint8_t board_id[36];
__attribute__((section(".ccm"))) const uint32_t board_hash;

__attribute__((constructor(1000))) void board_id_init(void)
{
	RCC->AHBENR |= RCC_AHBENR_CRCEN;
	__DSB();

	snprintf(
			(uint8_t *)board_id, 36,
			"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
			BOARD_ID[0], BOARD_ID[1], BOARD_ID[2],  BOARD_ID[3],
			BOARD_ID[4], BOARD_ID[5], BOARD_ID[6],  BOARD_ID[7],
			BOARD_ID[8], BOARD_ID[9], BOARD_ID[10], BOARD_ID[11]);

	CRC->CR |= CRC_CR_RESET;
	for (int idx = 0; idx < 3; idx++)
	{
		uint32_t id = ((uint32_t *)BOARD_ID)[idx];
		CRC->DR = id;
		__DSB();
	}

	*((uint32_t *)&board_hash) = CRC->DR;
}
