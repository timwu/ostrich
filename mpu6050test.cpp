/*
 * mpu6050test.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: tim
 */

#include <ch.h>
#include <hal.h>
#include <i2c.h>
#include <MPU6050.h>

#include "util.h";

int main(void) {
	int16_t ax, ay, az, gx, gy, gz;

	halInit();
	chSysInit();

	I2CConfig i2cConfig;
	i2cStart(&I2CD1, &i2cConfig);
	sdStart(&SD1, NULL);

	MPU6050 mpu6050;
	mpu6050.initialize();

	while (true) {
		mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		printf("%d, %d, %d\r\n", gx, gy, gz);
//		printf("gx = %d, gy = %d, gz = %d\r\n", gx, gy, gz);
		chThdSleepMilliseconds(1000);
	}
}




