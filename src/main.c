/*
 * main.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f3xx.h>
#include <stdio.h>

int main(void)
{
	fprintf(stderr, "Ready.\r\n");

	for (;;)
		__WFE();
}
