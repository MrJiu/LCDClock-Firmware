/*
 * main.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f3xx.h>
#include <stdio.h>

#include "board.h"

int main(void)
{
	board_init();

	volatile int val = fprintf(stderr, "Ready.\r\n");

	for (;;)
		__WFE();
}
