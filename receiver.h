/*
 * receiver.h
 *
 *  Created on: Mar 20, 2014
 *      Author: tim
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

void receiverSetup();
double receiverGetDouble(uint8_t channel);
uint8_t receiverGetBoolean(uint8_t channel);
uint16_t receiverGetRaw(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* RECEIVER_H_ */
