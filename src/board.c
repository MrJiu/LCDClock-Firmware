/*
 * board.c
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#include "board.h"

#include <dreamos-rt/gpio.h>
#include <stdio.h>
#include <stm32f1xx.h>

__attribute__((constructor(1001))) void board_init(void)
{
	static const int pinModes[] =
	{
			// GPIOA
			OUTPUT,						// PA0	LCD_D0
			OUTPUT,						// PA1	LCD_D1
			OUTPUT,						// PA2	LCD_D2
			OUTPUT,						// PA3	LCD_D3
			OUTPUT,						// PA4	LCD_D4
			OUTPUT,						// PA5	LCD_D5
			OUTPUT,						// PA6	LCD_D6
			OUTPUT,						// PA7	LCD_D7

			OUTPUT,						// PA8	USB_PU
			ALT_PU(0),					// PA9	DBG_TX
			ALT_PU(0),					// PA10	DBG_RX
			ANALOG,						// PA11	USB_DM
			ANALOG,						// PA12	USB_DP
			-1,							// PA13	DBG_TMS
			-1,							// PA14	DBG_TCK
			-1,							// PA15	DBG_TDI

			// GPIOB
			OUTPUT,						// PB0	LCD_RS
			ANALOG,						// PB1	ADC_BATT
			OUTPUT,						// PB2	LCD_RW
			-1,							// PB3	DBG_TDO
			-1,							// PB4	DBG_TRST
			OUTPUT,						// PB5	WL_RESET
			ALT_PU(0),					// PB6	LCD_BL
			OUTPUT,						// PB7	WL_EN

			ALT_OD(0),					// PB8	I2C_SCL
			ALT_OD(0),					// PB9	I2C_SDA
			ALT_PU(0),					// PB10	WL_TX
			ALT_PU(0),					// PB11	WL_RX
			OUTPUT,						// PB12	WL_BOOT
			OUTPUT,						// PB13	LCD_EN
			INPUT_PULLUP,				// PB14	ACK_BTN
			INPUT						// PB15	USB_VBUS
	};

	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	AFIO->MAPR = AFIO_MAPR_I2C1_REMAP;

	for (uint8_t pin = 0; pin < sizeof(pinModes) / sizeof(int); pin++)
	{
		int mode = pinModes[pin];
		if (mode > 0)
			pinMode(pin, mode);
		__DSB();
	}
}
