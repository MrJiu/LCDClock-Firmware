/*
 * pwm.h
 *
 *  Created on: Sep 24, 2017
 *      Author: technix
 */

#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

void analogWrite(uint8_t pin, uint32_t value);

__END_DECLS

#endif /* INCLUDE_PWM_H_ */
