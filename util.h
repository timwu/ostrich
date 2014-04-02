/*
 * util.h
 *
 *  Created on: Feb 4, 2014
 *      Author: tim
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <chprintf.h>
#define printf(...) chprintf((BaseSequentialStream *) &SD1, __VA_ARGS__)

#endif /* UTIL_H_ */
