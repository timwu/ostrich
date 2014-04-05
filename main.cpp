/*
 * main.c
 *
 *  Created on: Apr 2, 2014
 *      Author: tim
 */
#include <ch.h>
#include <hal.h>
#include "receiver.h"
#include "power.h"
#include "motors.h"
#include "util.h"

#define PITCH_CH 1
#define ROLL_CH 0
#define YAW_CH 3
#define THROTTLE_CH 2

static SerialConfig serialConfig = {
    UBRR_F(115200),
    USART_CHAR_SIZE_8
};

int main(void) {
  halInit();
  chSysInit();
  sdStart(&SD1, &serialConfig);

  receiverSetup();
  powerSetup();
  motorsSetup();

  while (1) {
    motorsSetControl(receiverGetFloat(PITCH_CH), receiverGetFloat(ROLL_CH),
        receiverGetFloat(YAW_CH), receiverGetFloat(THROTTLE_CH));
//    uint16_t throttle = receiverGetRaw(THROTTLE_CH);
//    uint16_t pitch = receiverGetRaw(PITCH_CH);
//    uint16_t roll = receiverGetRaw(ROLL_CH);
//    uint16_t yaw = receiverGetRaw(YAW_CH);
//    printf("%u, %u, %u, %u\r\n", throttle, pitch, roll, yaw);
//    printf("%u\r\n", receiverGetRaw(4));
    chThdSleepMilliseconds(50);
  }
}

