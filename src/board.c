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
#include <stm32f303xc_it.h>

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
			AFIO_PU(GPIO_MODE_AF7),		// PA9	DBG_TX
			AFIO_PU(GPIO_MODE_AF7),		// PA10	DBG_RX
			AFIO(GPIO_MODE_AF14),		// PA11	USB_DM
			AFIO(GPIO_MODE_AF14),		// PA12	USB_DP
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
			AFIO_PU(GPIO_MODE_AF5),		// PB6	LCD_BL
			OUTPUT,						// PB7	WL_EN

			AFIO_OD(GPIO_MODE_AF4),		// PB8	I2C_SCL
			AFIO_OD(GPIO_MODE_AF4),		// PB9	I2C_SDA
			AFIO_PU(GPIO_MODE_AF7),		// PB10	WL_TX
			AFIO_PU(GPIO_MODE_AF7),		// PB11	WL_RX
			OUTPUT,						// PB12	WL_BOOT
			OUTPUT,						// PB13	LCD_EN
			INPUT_PULLUP,				// PB14	ACK_BTN
			INPUT | GPIO_MODE_PUPD_PD,	// PB15	USB_VBUS
	};

	for (uint8_t pin = 0; pin < 32; pin++)
	{
		int mode = pinModes[pin];
		if (mode > 0)
			pinMode(pin, mode);
	}

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_TSC_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI4_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
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

__attribute__((section(".ccm"))) void (*line_interrupt[16])(void) = {NULL};
__attribute__((section(".ccmcode"))) void EXTI_IRQHandler(void)
{
	for (uint8_t idx = 0; idx < 16; idx++)
	{
		uint32_t flag = 1 << idx;
		if (EXTI->PR & flag)
		{
			EXTI->PR |= flag;
			if (line_interrupt[idx])
				line_interrupt[idx]();
		}
	}
}

__attribute__((alias("EXTI_IRQHandler"))) void EXTI0_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI1_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI2_TSC_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI3_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI4_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI9_5_IRQHandler(void);
__attribute__((alias("EXTI_IRQHandler"))) void EXTI15_10_IRQHandler(void);
