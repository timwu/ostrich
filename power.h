/*
 * power.h
 *
 *  Created on: Apr 1, 2014
 *      Author: tim
 */

#ifndef POWER_H_
#define POWER_H_

#define POWER_CHANNEL 4
#define POWER_PORT IOPORT6
#define POWER_PAD 6

#ifdef __cplusplus
extern "C" {
#endif

void powerSetup();

#ifdef __cplusplus
}
#endif

#endif /* POWER_H_ */
