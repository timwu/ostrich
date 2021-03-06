/*
 * receiver.h
 *
 *  Created on: Mar 20, 2014
 *      Author: tim
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#include <ch.h>

#ifdef __cplusplus
extern "C" {
#endif

void receiverSetup();
double receiverGetDouble(uint8_t channel);
bool receiverGetBoolean(uint8_t channel);
uint16_t receiverGetRaw(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* RECEIVER_H_ */
