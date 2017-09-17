/*
 * gpio.c
 *
 *  Created on: Sep 17, 2017
 *      Author: technix
 */

#include <dreamos-rt/gpio.h>
#include <stm32f3xx.h>

__attribute__((constructor)) void GPIO_Initialize(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
}

static inline GPIO_TypeDef *GPIO_GetPin(uint8_t pin, uint8_t *pin_id)
{
	static GPIO_TypeDef *const GPIO[] =
	{
			GPIOA,
			GPIOB,
			GPIOC,
			GPIOD,
			GPIOE,
			GPIOF
	};

	if (*pin_id)
		*pin_id = pin & 0x0f;
	return GPIO[(pin & 0xf0) >> 4];
}

uint16_t getPinMode(uint8_t pin);

void pinMode(uint8_t pin, uint16_t mode)
{
	uint8_t pinid = 0;
	GPIO_TypeDef *GPIO = GPIO_GetPin(pin, &pinid);
}

void digitalWrite(uint8_t pin, bool value);
bool digitalRead(uint8_t pin);
