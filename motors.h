/*
 * motors.h
 *
 *  Created on: Apr 2, 2014
 *      Author: tim
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

void motorsSetup();
void motorsSetControl(double pitch, double roll, double yaw, double throttle);

#ifdef __cplusplus
}
#endif

#endif /* MOTORS_H_ */
