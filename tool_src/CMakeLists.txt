# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

# Name your project
project(CharacterStone)

#device dependent config, here for arduino mega on linux
set(AVR_MCU atmega2560)
set(MCU_FREQ 16000000UL)
set(AVR_PROGRAMMER wiring)
set(AVR_PROGRAMMER_PORT /dev/ttyACM0)
#set(AVR_PROGRAMMER_BAUDRATE 19200)
set(PROGRAM_EEPROM False)

set(${Arduino_VARIANT} standard)
#find_package(Arduino REQUIRED COMPONENTS core_arduino)

#message(WARNING "TEST ${Arduino_INCLUDE_DIRS}")
include_directories("./IR_Decoder")
include_directories("./Master")
include_directories("./Slave")
include_directories("/usr/include/avr")

avr_add_library(IRDecoder ./IR_Decoder/IR_Decoder.c)

avr_add_executable(SlaveMain ./Slave/Slave.c)
#avr_add_executable(MasterMain ./Master/Main.c)

target_link_libraries(SlaveMain.elf IRDecoder)

