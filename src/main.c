/*
 * main.c
 *
 *  Created on: Aug 23, 2017
 *      Author: technix
 */

#include <stm32f1xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <dreamos-rt/gpio.h>

#include "board.h"
#include "pwm.h"
#include "lcd.h"
#include "i2c.h"

#define DHT_ADDR 0x5c

extern uint32_t __mem_begin, __mem_size;

int DISP;
clock_t last_update;

char lcd_line[2][20];

#include "charmap.h"

__attribute__((packed)) struct dht12
{
	uint8_t humid_int;
	uint8_t humid_frac;
	uint8_t temp_int;
	uint8_t temp_frac;
	uint8_t cksum;
};

int main(void)
{
	i2c_open();

	DISP = open("/dev/lcd", O_RDWR);
	ioctl(DISP, IOCTL_LCD_SET_CURSOR, false);
	ioctl(DISP, IOCTL_LCD_SET_CHARACTER, &degree);
	ioctl(DISP, IOCTL_LCD_SET_CHARACTER, &charged);
	ioctl(DISP, IOCTL_LCD_SET_CHARACTER, &battery_empty);
	ioctl(DISP, IOCTL_LCD_SET_CHARACTER, &battery_half_empty);
	ioctl(DISP, IOCTL_LCD_SET_CHARACTER, &battery_half_full);
	ioctl(DISP, IOCTL_LCD_SET_CHARACTER, &battery_full);
	ioctl(DISP, IOCTL_LCD_CLEAR);
	ioctl(DISP, IOCTL_LCD_HOME);

	fprintf(stderr,
			"\r\n\033[3J\033[H\033[2J"
			"SushiBits Connected Clock, M205v1.\r\n"
			"Software version 0.1, compiled " __DATE__ " " __TIME__ "\r\n"
			"Copyright (c) 2017 Max Chan. All rights reserved.\r\n");
	//fprintf(stderr, "Hardware ID: %s <0x%08x>\r\n", board_id, board_hash);
	fprintf(stderr, "%lu bytes heap memory allocated. Total %lu bytes.\r\n", (uint32_t)sbrk(0) - __mem_begin, __mem_size);
	fprintf(stderr, "Probing I2C bus...\r\n");
	i2c_probe();
	fprintf(stderr, "Ready.\r\n");

	analogWrite(0, 0x8000);
	last_update = clock() - CLOCKS_PER_SEC;

	for (;;)
	{
		clock_t now = clock();
		if (now - last_update >= CLOCKS_PER_SEC / 2)
		{
			last_update = now;

			struct dht12 dht = {0};
			uint8_t addr = 0;
			volatile int bytes = i2c_transfer(DHT_ADDR, &addr, 1, &dht, sizeof(dht));

			memset(lcd_line, 0, sizeof(lcd_line));

			time_t t = time(NULL);
			struct tm tm = {0};
			localtime_r(&t, &tm);
			strftime(lcd_line[0], 17, "%m-%d %H:%M:%S  ", &tm);
			snprintf(lcd_line[1], 17, "% 3d.%1u#C RH% 3u.%1u%%",
					dht.temp_int * ((dht.temp_frac & 0x80) ? -1 : 1), dht.temp_frac & 0xf,
					dht.humid_int, dht.humid_frac);
			char *degree = strchr(lcd_line[1], '#');
			*degree = 1;

			lcd_line[0][15] = digitalRead(0x1f) ? 2 : 5;

			lcd_location(DISP, 0, 0);
			write(DISP, lcd_line[0], 16);
			lcd_location(DISP, 0, 1);
			write(DISP, lcd_line[1], 16);
		}

		__WFE();
	}
}
