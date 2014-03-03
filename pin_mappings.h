/*
 * pin_mappings.h
 *
 *  Created on: Mar 1, 2014
 *      Author: tim
 */

#ifndef PIN_MAPPINGS_H_
#define PIN_MAPPINGS_H_

#include <hal.h>

#define int_port(i) (pc_int_pins[(i)].port)
#define int_pad(i) (pc_int_pins[(i)].pad)
#define readIntPad(i) (palReadPad(int_port(i), int_pad(i)))
#define setIntPadMode(i) (palSetPadMode(int_port(i), int_pad(i), PAL_MODE_INPUT))

typedef struct {
  ioportid_t port;
  uint8_t pad;
} pin_spec;

pin_spec pc_int_pins[] = {
#ifdef __AVR_ATmega1280__
    {IOPORT2, 0},
    {IOPORT2, 1},
    {IOPORT2, 2},
    {IOPORT2, 3},
    {IOPORT2, 4},
    {IOPORT2, 5},
    {IOPORT2, 6},
    {IOPORT2, 7},
#else
#error "Can't determine chip for pin mapping."
#endif
};


#endif /* PIN_MAPPINGS_H_ */
