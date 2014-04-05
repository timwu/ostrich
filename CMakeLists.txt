set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)

project(OSTRICH C CXX)

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

file(GLOB I2CDEV_LIB_SRCS
    "i2cdevlib/Arduino/I2Cdev/*.cpp"
)

file(GLOB MPU6050_SRCS
   "i2cdevlib/Arduino/MPU6050/*.cpp"
)

include_directories(
   "ChibiOS/os/kernel/include"
   "ChibiOS/os/hal/platforms/AVR"
   "ChibiOS/os/hal/include"
   "ChibiOS/os/ports/GCC/AVR"
   "ChibiOS/os/various"
   "ChibiOS/boards/ARDUINO_MEGA"
   "i2cdevlib/Arduino/I2Cdev"
   "i2cdevlib/Arduino/MPU6050"
   "${PROJECT_SOURCE_DIR}"
)

set(mpu6050test_SRCS
   ${CHIBIOS_KERNEL_SRCS}
   ${CHIBIOS_HAL_SRCS}
   ${CHIBIOS_VARIOUS_SRCS}
   ${I2CDEV_LIB_SRCS}
   ${MPU6050_SRCS}
   "mpu6050test.cpp"
   "ChibiOS/os/various/chprintf.c"
)

generate_avr_firmware(mpu6050test
   SERIAL minicom -b 38400 -D @SERIAL_PORT@
)

set(ostrich_SRCS
   ${CHIBIOS_KERNEL_SRCS}
   ${CHIBIOS_HAL_SRCS}
   "main.cpp"
   "ChibiOS/os/various/chprintf.c"
   "receiver.c"
   "power.c"
   "pins.c"
   "motors.c"
)

generate_avr_firmware(ostrich
   SERIAL minicom -b 115200 -D @SERIAL_PORT@
)