/*
 * pilot.h
 *
 *  Created on: Apr 6, 2014
 *      Author: tim
 */

#ifndef PILOT_H_
#define PILOT_H_

#include <ch.h>

#ifdef __cplusplus
extern "C" {
#endif

void pilotSetup();
double pilotGetPitch();
double pilotGetRoll();
double pilotGetYaw();
double pilotGetThrottle();
bool_t pilotGetPower();

#ifdef __cplusplus
}
#endif

#endif /* PILOT_H_ */
