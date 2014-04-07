/*
 * power.h
 *
 *  Created on: Apr 1, 2014
 *      Author: tim
 */

#ifndef POWER_H_
#define POWER_H_

#include <ch.h>
#include <hal.h>

#define POWER_PORT IOPORT6
#define POWER_PAD 6

#ifdef __cplusplus
extern "C" {
#endif

void powerSetup();
bool_t powerIsOn();

#ifdef __cplusplus
}
#endif

#endif /* POWER_H_ */
