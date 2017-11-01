/*
 * pwm.c
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#include "pwm.h"

#include <stm32f1xx.h>

__attribute__((constructor(1000))) void pwm_init(void)
{
	// We are only using timer 8 here.
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->CNT = 0;
	TIM4->PSC = 0;
	TIM4->ARR = 0xffff;
	TIM4->RCR = 0;

	TIM4->CCMR1 = 0x68;
	TIM4->CCER = TIM_CCER_CC1E;
	TIM4->BDTR = TIM_BDTR_MOE;
	TIM4->CR1 |= TIM_CR1_CEN;
	__DSB();
}

void analogWrite(__attribute__((unused)) uint8_t pin, uint32_t value)
{
	TIM4->CCR1 = value;
}
