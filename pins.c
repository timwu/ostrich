/*
 * pins.c
 *
 *  Created on: Apr 2, 2014
 *      Author: tim
 */
#include <hal.h>
#include "pins.h"

typedef struct {
  ioportid_t port;
  uint8_t pad;
} pin_spec;

static pin_spec pc_int_pins[] = {
#ifdef __AVR_ATmega1280__
    {IOPORT2, 0},
    {IOPORT2, 1},
    {IOPORT2, 2},
    {IOPORT2, 3},
    {IOPORT2, 4},
    {IOPORT2, 5},
    {IOPORT2, 6},
    {IOPORT2, 7},
    {IOPORT5, 0},
    {IOPORT9, 0},
    {IOPORT9, 1},
    {IOPORT9, 2},
    {IOPORT9, 3},
    {IOPORT9, 4},
    {IOPORT9, 5},
    {IOPORT9, 6},
    {IOPORT10, 0},
    {IOPORT10, 1},
    {IOPORT10, 2},
    {IOPORT10, 3},
    {IOPORT10, 4},
    {IOPORT10, 5},
    {IOPORT10, 6},
    {IOPORT10, 7},
#else
#error "Can't determine chip for pin mapping."
#endif
};

uint8_t readIntPad(uint8_t interrupt) {
  return palReadPad(pc_int_pins[interrupt].port, pc_int_pins[interrupt].pad);
}
