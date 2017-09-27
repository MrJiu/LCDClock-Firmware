/*
 * main.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f3xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "board.h"
#include "pwm.h"

extern uint32_t __mem_begin, __mem_size;
extern uint32_t __ccmram_size, __ccm_size;

FILE *lcd;

int main(void)
{
	lcd = fopen("/dev/lcd", "rw+");
	setvbuf(lcd, NULL, _IONBF, 0);

	fprintf(stderr,
			"\r\n\033[3J\033[H\033[2J"
			"SushiBits Connected Clock, M205v1.\r\n"
			"Software version 0.1, compiled " __DATE__ " " __TIME__ "\r\n"
			"Copyright (c) 2017 Max Chan. All rights reserved.\r\n");
	fprintf(stderr, "Hardware ID: %s <0x%08x>\r\n", board_id, board_hash);
	fprintf(stderr, "%lu bytes stack memory allocated. Total %lu bytes.\r\n", __ccm_size, __ccmram_size);
	fprintf(stderr, "%lu bytes heap memory allocated. Total %lu bytes.\r\n", (uint32_t)sbrk(0) - __mem_begin, __mem_size);
	fprintf(stderr, "Ready.\r\n");
	fprintf(lcd, "Ready.");

	analogWrite(0, 0x8000);

	for (;;)
	{
		sleep(1);
	}
}
