include Arduino.mk

LIBS=core DallasTemperature OneWire 
PORT=COM13
CPU=atmega328p
BR=57600
CPPFLAGS+=-DARDUINO_AVR_PRO -DARDUINO_ARCH_AVR
VARIANT=standard
PROGRAMMER=arduino

WIRE_LIB_PATH=$(ARD_HOME)/hardware/arduino/avr/libraries/Wire
include lib.mk

ifeq ($(filter %-pc-cygwin,$(MAKE_HOST)),)
CPPFLAGS+=-I$(WIRE_LIB_PATH) -I$(WIRE_LIB_PATH)/utility
else
CPPFLAGS+=-I$(shell cygpath -m $(WIRE_LIB_PATH)) -I$(shell cygpath -m $(WIRE_LIB_PATH)/utility)
endif

libWire.a: Wire/Wire.o  Wire/twi.o
	$(AR) rcs $@ $^

Wire/Wire.o:$(WIRE_LIB_PATH)/Wire.cpp |$(CURDIR)/Wire/
	$(CXX_CMD)

Wire/twi.o:$(WIRE_LIB_PATH)/utility/twi.c |$(CURDIR)/Wire/
	$(CC_CMD)

$(CURDIR)/Wire/:
	mkdir -p $@


CPPFLAGS+=-Wall -Wextra -I. -Os -fno-exceptions -ffunction-sections -fdata-sections

main.elf: main.o libDallasTemperature.a libOneWire.a libWire.a libcore.a 

