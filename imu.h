/*
 * imu.h
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */

#ifndef IMU_H_
#define IMU_H_

void imuSetup();

double imuGetPitch();
double imuGetRoll();

double imuGetYawRate();

#endif /* IMU_H_ */
