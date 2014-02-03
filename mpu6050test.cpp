/*
 * mpu6050test.cpp
 *
 *  Created on: Feb 2, 2014
 *      Author: tim
 */

#include <ch.h>
#include <chprintf.h>
#include <hal.h>
#include <i2c.h>
#include <MPU6050.h>

#define printf(...) chprintf((BaseSequentialStream *) &SD1, __VA_ARGS__)

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
		printf("ax = %d, ay = %d, az = %d\r\n", ax, ay, az);
		printf("gx = %d, gy = %d, gz = %d\r\n", gx, gy, gz);
		chThdSleepMilliseconds(1000);
	}
}




