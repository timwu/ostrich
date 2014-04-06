/*
 * gyro.h
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */

#ifndef GYRO_H_
#define GYRO_H_

void gyroSetup();
int16_t gyroGetPitchRotation();
int16_t gyroGetRollRotation();
int16_t gyroGetYawRotation();

#endif /* GYRO_H_ */
