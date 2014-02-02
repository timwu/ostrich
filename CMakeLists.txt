set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)

project(I2CPROBE C CXX)

set(ARDUINO_AVRDUDE_CONFIG_PATH /usr/local/etc/avrdude.conf)
set(ARDUINO_DEFAULT_BOARD mega)
set(ARDUINO_DEFAULT_PORT /dev/tty.usbserial-A6008hgx)

file(GLOB CHIBIOS_KERNEL_SRCS
	"ChibiOS/os/kernel/src/*.c"
	"ChibiOS/os/kernel/src/*.h"
	"ChibiOS/os/ports/GCC/AVR/*.c"
)

file(GLOB CHIBIOS_HAL_SRCS
	"ChibiOS/boards/ARDUINO_MEGA/board.c"
	"ChibiOS/os/hal/src/*.c"
	"ChibiOS/os/hal/platforms/AVR/*.c"
)

include_directories(
	"ChibiOS/os/kernel/include"
	"ChibiOS/os/hal/platforms/AVR"
	"ChibiOS/os/hal/include"
   "ChibiOS/os/ports/GCC/AVR"
   "ChibiOS/boards/ARDUINO_MEGA"
)

set(i2cprobe_SRCS ${ChibiOS_KERNEL_SRCS} ${ChibiOS_HAL_SRCS}
)

generate_avr_firmware(i2cprobe)
