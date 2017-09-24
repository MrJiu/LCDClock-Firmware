/*
 * system_stm32f303cx.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f3xx.h>
#include <stm32f303xc_it.h>

#ifndef HSE_VALUE
#define HSE_VALUE 0
#endif

#define HSI_VALUE 8000000 //  8MHz
#define LSI_VALUE   40000 // 40kHz

uint32_t SystemCoreClock;

void SystemInit(void)
{
	// Turn on HCLK oscillator and PLL on it, at 72MHz PLLCLK.
	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));
	SET_FIELD(RCC->CFGR, RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC, RCC_CFGR_PLLMUL9 | RCC_CFGR_PLLSRC_HSE_PREDIV);
	SET_FIELD(RCC->CFGR2, RCC_CFGR2_PREDIV, RCC_CFGR2_PREDIV_DIV1);
	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Prepare the system for the clock switch.
	RCC->AHBENR |= RCC_AHBENR_FLITFEN;
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1;
	SET_FIELD(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);

	// Clock switch
	SET_FIELD(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
	while (RCC->CFGR & RCC_CFGR_SWS != RCC_CFGR_SWS_PLL);

	// Turn on LSE if not already on.
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_DBP;
	if (!(RCC->BDCR & RCC_BDCR_RTCEN))
	{
		RCC->BDCR |= RCC_BDCR_LSEON;
		while (!(RCC->BDCR & RCC_BDCR_LSERDY));
		SET_FIELD(RCC->BDCR, RCC_BDCR_RTCSEL, RCC_BDCR_RTCSEL_LSE);
		__DSB();
		RCC->BDCR |= RCC_BDCR_RTCEN;
	}
}

void SystemCoreClockUpdate(void)
{
	// Determine the SYSCLK
	switch (RCC->CFGR & RCC_CFGR_SWS)
	{
	case RCC_CFGR_SWS_HSI:
		SystemCoreClock = HSI_VALUE;
		break;

	case RCC_CFGR_SWS_HSE:
		SystemCoreClock = HSE_VALUE;
		break;

	case RCC_CFGR_SWS_PLL:
		// Determine the PLL input.
		if (RCC->CFGR & RCC_CFGR_PLLSRC)
		{
			uint32_t HSE_PREDIV = (RCC->CFGR2 & RCC_CFGR2_PREDIV) + 1;
			SystemCoreClock = HSE_VALUE / HSE_PREDIV;
		}
		else
		{
			SystemCoreClock = HSI_VALUE / 2;
		}

		// Determine the PLL multiplication factor.
		uint32_t PLL_MULT = ((RCC->CFGR & RCC_CFGR_PLLMUL) >> 18) + 2;
		PLL_MULT = (PLL_MULT > 16) ? 16 : PLL_MULT;
		SystemCoreClock *= PLL_MULT;
		break;
	}

	// Determine the HCLK divider
	uint32_t HCLK_PRE = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
	const static uint32_t HCLK_DIV[] =
	{
			1, 1, 1, 1,  1,  1,   1,   1,
			2, 4, 8, 16, 64, 128, 256, 512
	};
	SystemCoreClock /= HCLK_DIV[HCLK_PRE];
}
