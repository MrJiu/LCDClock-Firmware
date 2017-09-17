/*
 * usart.h
 *
 *  Created on: Aug 26, 2017
 *      Author: technix
 */

#ifndef SYSTEM_INCLUDE_DREAMOS_RT_USART_H_
#define SYSTEM_INCLUDE_DREAMOS_RT_USART_H_

#include <dreamos-rt/device.h>
#include <stm32f303xc_it.h>
#include <stm32f3xx.h>

#define USART_READ_BUFFER_SIZE		64
#define USART_WRITE_BUFFER_SIZE		64
#define USART_DEFAULT_BAUDRATE		115200

// USART ioctl's

#define IOCTL_USART_GET_BAUDRATE		0
#define IOCTL_USART_SET_BAUDRATE		1

#endif /* SYSTEM_INCLUDE_DREAMOS_RT_USART_H_ */
