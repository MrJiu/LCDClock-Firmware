/*
 * board.c
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#include "board.h"

#include <dreamos-rt/gpio.h>

void board_init(void)
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
			AFIO_PU(GPIO_MODE_AF2),	// LCD_BL
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
