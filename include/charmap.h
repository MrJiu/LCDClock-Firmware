/*
 * charmap.h
 *
 *  Created on: Sep 28, 2017
 *      Author: technix
 */

#ifndef INCLUDE_CHARMAP_H_
#define INCLUDE_CHARMAP_H_

#include "lcd.h"

static lcd_character_t degree =
{
		.character = 1,
		.bitmap =
		{
				0b00110,
				0b01001,
				0b01001,
				0b00110,
				0b00000,
				0b00000,
				0b00000,
				0b00000
		}
};

static lcd_character_t charged =
{
		.character = 2,
		.bitmap =
		{
				0b01010,
				0b01010,
				0b11111,
				0b10001,
				0b01110,
				0b00100,
				0b00100,
				0b00000
		}
};

static lcd_character_t battery_empty =
{
		.character = 4,
		.bitmap =
		{
				0b01110,
				0b11111,
				0b10001,
				0b10001,
				0b10011,
				0b10111,
				0b11111,
				0b00000
		}
};

static lcd_character_t battery_half_empty =
{
		.character = 5,
		.bitmap =
		{
				0b01110,
				0b11111,
				0b10001,
				0b10011,
				0b10111,
				0b11111,
				0b11111,
				0b00000
		}
};

static lcd_character_t battery_half_full =
{
		.character = 6,
		.bitmap =
		{
				0b01110,
				0b11111,
				0b10011,
				0b10111,
				0b11111,
				0b11111,
				0b11111,
				0b00000
		}
};

static lcd_character_t battery_full =
{
		.character = 7,
		.bitmap =
		{
				0b01110,
				0b11111,
				0b10111,
				0b11111,
				0b11111,
				0b11111,
				0b11111,
				0b00000
		}
};

#endif /* INCLUDE_CHARMAP_H_ */
